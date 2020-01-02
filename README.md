# Simplified-NAT

To run the program:
1) Download the zip.
2) Extract it.
3) Copy the nat and flow input files in this extracted directory and navigate to this directory in the command line.
4) Compile the program using the command "g++ -o main main.cpp"
5) Run the program using the command "main <nat_file_name> <flow_file_name>". Example "main NAT.txt FLOW.txt"

A new file named OUTPUT will be created in the directory containing the output. 

NOTE: Include file formats in the file names. 

If I had more time, I would have 
1) run some more tests
2) designed the algorithm in a way that could handle inputs like 10.*.10:8080
3) tried to make the program even more efficient
4) measure the performance of different STL containers with chrono timer. 


