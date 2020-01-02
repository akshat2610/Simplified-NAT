#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <unordered_map>
using namespace std;


/**
 * This programs assumes that it is always provided with valid inputs.
 * As per the specified requirement, possible valid input formats are:
      10.0.1.1:8080,192.168.0.1:80
      *:8081,192.168.0.1:80
      10.0.1.2:*,192.168.0.1:80
 * Hence, input formats such as:
      10.*:8080
      *.10:8080
      10.*.10:8080
   will be considered invalid
*/

/**
 * Populates the unordered map data structure from the input file containing the NAT
 * @param NAT the unordered map data structure reference
 * @param NAT_input_file name of the input file containing the NAT
 * @precondition the input file contains only valid inputs
*/
void readNAT(unordered_map<string, string> &NAT, string NAT_input_file);

/**
 * Populates the flow list from the input file containing the flow
 * @param flow the flow list reference
 * @param flow_input_file name of the input file containing the flow
 * @precondition the input file contains only valid inputs
*/
void readFlow(list<string> &flow, string flow_input_file);

/**
 * Loops over the flow list to look for each address in the NAT and output it to the output stream
 * @param NAT the unordered map data structure reference
 * @param flow the flow list reference
*/
void match(unordered_map<string, string> &NAT, list<string> &flow);

int main(int argc, char** argv){
  if (argc < 3){
    cout << "Not enough arguments " << endl;
    exit(1);
  }

  unordered_map<string, string> NAT;
  list<string> flow;

  readNAT(NAT, argv[1]);
  readFlow(flow, argv[2]);
  match(NAT, flow);

  return 0;
}

void match(unordered_map<string, string> &NAT, list<string> &flow){
  ofstream output_stream;
  output_stream.open("OUTPUT.txt");

  if (output_stream.fail()){
    cout << "Could not open file: OUTPUT.txt" << endl;
    exit(1);
  }

  list<string>::iterator flow_iter;

  for (flow_iter = flow.begin(); flow_iter != flow.end(); flow_iter++){
    string full_key = *flow_iter;
    //If complete ip:port pair found --> write to output stream
    if (NAT.find(full_key) != NAT.end())
      output_stream << full_key << "->" << NAT.at(full_key) << endl;
    //Else if not found --> look for either ip or pair
    else{
      size_t colon_pos = full_key.find(":");
      string ip_part = full_key.substr(0, colon_pos-1);
      string port_part = full_key.substr(colon_pos+1);

      if (NAT.find(ip_part) != NAT.end())
        output_stream << full_key << "->" << NAT.at(ip_part) << endl;
      else if (NAT.find(port_part) != NAT.end())
        output_stream << full_key << "->" << NAT.at(port_part) << endl;
      else
        output_stream << "No nat match for " << full_key << endl;
    }
  }
  output_stream.close();
}

void readNAT(unordered_map<string, string> &NAT, string NAT_input_file){
  ifstream input_stream;
  input_stream.open(NAT_input_file);

  if (input_stream.fail()){
    cout << "Could not load file: " << NAT_input_file << endl;
    exit(1);
  }

  while (!input_stream.eof()){
    string original_address, new_address;
    getline (input_stream, original_address, ',');
    getline (input_stream, new_address);

    if (original_address.length() != 0){
      size_t asterisk_pos = original_address.find("*");
      //if the original address does not contain an asterisk
      if (asterisk_pos == string::npos)
        NAT.insert(pair<string, string>(original_address, new_address));
      //else if the original address contains an asterisk
      else{
        //By the

        //If the asterisk is at the beginning --> just insert the PORT and new address.
        if (asterisk_pos == 0)
          NAT.insert(pair<string, string>(original_address.substr(asterisk_pos+2), new_address));
        //Else if the asterisk is at the end --> just insert the IP and new address
        else
          NAT.insert(pair<string, string>(original_address.substr(0, asterisk_pos), new_address));
      }
    }
  }

  input_stream.close();
}

void readFlow(list<string> &flow, string flow_input_file){
  ifstream input_stream;
  input_stream.open(flow_input_file);

  if (input_stream.fail()){
    cout << "Could not open file: " << flow_input_file << endl;
    exit(1);
  }

  while (!input_stream.eof()){
    string pair;
    getline (input_stream, pair);
    if (pair.length() != 0) flow.push_back(pair);
  }

  input_stream.close();
}
