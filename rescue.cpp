// Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#include <string>
#include <vector>
#include <iostream>
#include <getopt.h>
#include <algorithm>
#include <deque>
#include <list>
#include <cstdint>
#include <sstream>

using namespace std;

    enum class RoutingScheme { None, Stack, Queue }; // construct routing scheme;
    enum class OutputFormat { Map, List }; // construct output format;

    // construct a combination;
    struct ProgramOptions {
        RoutingScheme routing = RoutingScheme::None;
        OutputFormat output = OutputFormat::Map;
    };
    
    bool solution = false; // is there a path;

class Rescue{
    private:
        vector<vector<char>> rescue_path; // dirction;

        ProgramOptions options; // get a default options;

        // 'help' information;
        void printHelp(char *argv[]) {
            std::cout << "Usage: " << argv[0] << " [options]\n"
                    << "Options:\n"
                    << "  --stack, -s\tUse the stack-based routing scheme\n"
                    << "  --queue, -q\tUse the queue-based routing scheme\n"
                    << "  --output (M|L), -o (M|L)\tSpecifies the output file format (M for map, L for list)\n"
                    << "  --help, -h\tDisplay this help message and exit\n";
        }

        vector<vector<char>> map; // map mode;
        char imode; // Map input mode:
        uint32_t roomNum; // number of room in the map
        uint32_t roomLength; // length of room
        uint32_t tile; // number of tiles discovered;

        string validChars = "#!0123456789SC"; // valid char, `.` is set as default;

        // read map mode;
        void readMapMode(vector<vector<char>> &map_in, vector<vector<char>> &rescue_path_in, const uint32_t &roomNum, const uint32_t &roomLength){
            string temp_line; // read each line from a file;
            uint32_t curr_room = 0; // current room;
            uint32_t curr_cell = 0; // current cell in a room;

            // read in char;
            while (getline(cin, temp_line) && curr_room < roomNum){
                if (temp_line[0] == '/' || temp_line.empty()) continue; // skip the comment;

                // fill current room;
                for (char c : temp_line){
                    if (c == '.'); // if the node is '.' continue to read the next one;
                    else if (validChars.find(c) != string::npos) {
                        if (c == 'S'){
                            rescue_path_in[curr_room][curr_cell] = 'Y'; // mark this node is discovered;
                            Marco = {curr_room, curr_cell / roomLength, curr_cell % roomLength};
                            deque_route.push_back(Marco);
                        }
                        map_in[curr_room][curr_cell] = c;
                    }
                    else { // other char;
                        cerr << "Unknown map character";
                        exit(1);
                    }
                    curr_cell++; // move to the next node;
                    // check if it is reading the next room;
                    if (curr_cell == roomLength * roomLength){
                        curr_cell = 0;
                        curr_room++;
                        if (curr_room >= roomNum) break;
                    }
                }
            }
        };

        // read list mode;
        void readListMode(vector<vector<char>> &map_in, vector<vector<char>> & rescue_path_in, const uint32_t &roomNum, const uint32_t &roomLength){
            string temp_line; // read each coordinate;

            while (getline(cin, temp_line)){
                if (temp_line[0] == '/' || temp_line.empty()) continue;

                // remove parentheses and parse the contents;
                stringstream ss(temp_line);
                char ch, c; // useless char, validChars;
                uint32_t room, row, col;
                ss >> ch >> room >> ch >> row >> ch >> col >> ch >> c; // e.g. (0,0,1,C)
                
                // error coordinate;
                if (room >= roomNum){
                    cerr << "Invalid room number\n";
                    exit(1);
                }; // room number is incorrect;
                if (row >= roomLength){
                    cerr << "Invalid row number\n";
                    exit(1);
                }
                if (col >= roomLength){
                    cerr << "Invalid column number\n";
                    exit(1);
                } // extend length;
                if (c == '.') continue; // if ch is '.' continue;
                else if (validChars.find(c) != string::npos){ // replace with validchars;
                    if (c == 'S'){
                        rescue_path_in[room][row * roomLength + col] = 'Y'; // mark this node is discovered; 
                        Marco = {room, row, col};
                        deque_route.push_back(Marco);
                    }
                    map_in[room][row * roomLength + col] = c;
                }
                else {
                    cerr << "Unknown map character";
                    exit(1);
                }
            }
        };

        struct Coord {
            uint32_t room, row, col;
        };

        Coord Marco; // record the starting node;
        Coord end; // record the end
        bool findC = false; // if 'C' is found;

        std::deque<Coord> deque_route; // store the 

        void pushIfValid(uint32_t room, uint32_t row, uint32_t col, char dir) {
            // make sure the positive index is valid;
            if (row < roomLength && col < roomLength) {
                int currIndex = row * roomLength + col;
                char & node = map[room][currIndex];
                char & visited = rescue_path[room][currIndex];
                // if the room exist;
                if (visited == 'N' && node != '#' && node != '!'){
                    visited = dir; // record information in the new node;
                    deque_route.push_back({room, row, col});
                    tile++;
                }

                // if we already find 'C';
                if (node == 'C'){
                    findC = true; // record 'C' is found;
                    end = {room, row, col}; // record 'C' position;
                    return;
                }
            }
        };

        // record the path;
        std::deque<Coord> path;

        void reconstructPath(Coord& end) {
            Coord curr = end; // start from C;
            while (curr.room != Marco.room || curr.row != Marco.row || curr.col != Marco.col){
                char trac = rescue_path[curr.room][curr.row * roomLength + curr.col]; // find the direction;
                switch(trac){
                    case 'n':
                        map[curr.room][(curr.row + 1) * roomLength + curr.col] = 'n';
                        curr = {curr.room, curr.row + 1, curr.col};
                        break;
                    case 's':
                        map[curr.room][(curr.row - 1) * roomLength + curr.col] = 's';
                        curr = {curr.room, curr.row - 1, curr.col};
                        break;
                    
                    case 'e':
                        map[curr.room][curr.row * roomLength + curr.col - 1] = 'e';
                        curr = {curr.room, curr.row, curr.col - 1};
                        break;
                    
                    case 'w':
                        map[curr.room][curr.row * roomLength + curr.col + 1] = 'w';
                        curr = {curr.room, curr.row, curr.col + 1};
                        break;
                    
                    default:
                        uint32_t orig_room = static_cast<uint32_t>(trac);
                        map[orig_room][curr.row * roomLength + curr.col] = 'p';
                        curr = {orig_room, curr.row, curr.col};
                        break;
                }
                path.push_front(curr);
            }
        };
        
    public:

        Rescue():imode(0), roomNum(0), roomLength(0), tile(0), 
                Marco({0,0,0}),end({0,0,0}){ }; // constructor;

        void getMode(int argc, char * argv[], ProgramOptions &options); // get the mode message;

        void readMap(); // read the castle map;

        bool routSchemes(const ProgramOptions& options); // tow routing schemes to find the route;

        void outputMap(const ProgramOptions& options); // output the solution on the map;

};

void Rescue::getMode(int argc, char * argv[], ProgramOptions &options) {
    opterr = false; // handle the error message bymyself;
    int routingCount = 0; // count how many times "M | L" occur;
    int index = 0; // identify which option was encountered;
    int choice;
    option long_options[] = { // multiple options
        { "stack", no_argument, nullptr, 's'},
        { "queue", no_argument, nullptr, 'q'},
        { "output", required_argument, nullptr, 'o'},
        { "help", no_argument, nullptr, 'h'},
        { nullptr, 0, nullptr, '\0' }, // end of the array;
    };  // long_options[]

    // TODO: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &index)) != -1) {
        switch (choice) {
            case 's':
                options.routing = RoutingScheme::Stack;
                routingCount++;
                break;
            case 'q':
                options.routing = RoutingScheme::Queue;
                routingCount++;
                break;
            case 'o':
                if (std::string(optarg) == "M") {
                    options.output = OutputFormat::Map;
                } 
                if (std::string(optarg) == "L") {
                    options.output = OutputFormat::List;
                }
                break;
            case 'h':
                printHelp(argv);
                exit(0);
            default:
                cerr << "Unknown command line option" << endl;
                exit(1);
        }
    }

    if (routingCount == 0) {
        std::cerr << "Stack or queue must be specified" << std::endl;
        exit(1);
    }
    else if (routingCount > 1){
        cerr << "Stack or queue can only be specified once" << endl;
        exit(1);
    }
}; // getMode()

void Rescue::readMap(){
    cin >> imode >> roomNum >> roomLength; // reading in map mode, room num, and room length;

    // make sure room num is between [1, 10];
    if (roomNum < 1 || roomNum > 10){
        cerr << "Invalid room number\n";
        exit(1); 
    };

    // the length side of every room is not less than 1;
    if (roomLength < 1){
        cerr << "Invalid row number\nInvalid row number\n";
        exit(1);
    };

    // set room and room length;
    map.resize(roomNum, vector<char>(roomLength * roomLength, '.')); // resize each room;
    rescue_path.resize(roomNum, vector<char>(roomLength * roomLength, 'N')); // set all tile is not visited;

    // read the following map;
    if (imode == 'M'){
        readMapMode(map, rescue_path, roomNum, roomLength); // read in map information;
    }
    if (imode == 'L'){
        readListMode(map, rescue_path, roomNum, roomLength); // read in map information;
    }

};

bool Rescue::routSchemes(const ProgramOptions& options) {
        bool useStack = options.routing == RoutingScheme::Stack;
        tile = 1; // start from Marco

        if (deque_route.empty()) {
            return false;  // No starting point
        }

        while (!findC && !deque_route.empty()) {
            Coord current = useStack ? deque_route.back() : deque_route.front();
            if (useStack) {
                deque_route.pop_back();
            } else {
                deque_route.pop_front();
            }
            
            int currentIndex = current.row * roomLength + current.col;
            char & curNode = map[current.room][currentIndex];

            if (curNode >= '0' && curNode <= '9'){ // if the node is a pipe;
                uint32_t pipe_room = static_cast<uint32_t>(curNode - '0');
                if(pipe_room >= roomNum) continue;
                char & pipe_node = rescue_path[pipe_room][currentIndex];
                char temp = map[pipe_room][currentIndex]; // the correspond map position;
                if (temp != '#' && temp != '!' && pipe_node == 'N'){
                    pipe_node = static_cast<char>(current.room); // record which room from;
                    deque_route.push_back({pipe_room, current.row, current.col});
                    tile++;
                }
                
                if (temp == 'C') {
                    findC = true;
                    end = {pipe_room, current.row, current.col};
                    break;
                }
            }
            // Check adjacent nodes: north, east, south, west
            else{
                pushIfValid(current.room, current.row - 1, current.col, 'n');
                pushIfValid(current.room, current.row, current.col + 1, 'e');
                pushIfValid(current.room, current.row + 1, current.col, 's');
                pushIfValid(current.room, current.row, current.col - 1, 'w');
            }    
        }

        if (findC) { // if a correct path was found;
            reconstructPath(end);
            // Output or use the path as needed
            if (options.output == OutputFormat::Map){
                std::cout << "Start in room " << Marco.room << ", row " << Marco.row << ", column " << Marco.col << '\n';
            }
            solution = true;
        }
        else { // if there is no solution;
            cout << "No solution, " << tile << " tiles discovered.\n";
        }

        return findC;    
};

void Rescue::outputMap(const ProgramOptions & options){
    bool useMap = options.output == OutputFormat::Map; // determin the output type;

    // cout map;
    if (useMap){
        for (uint32_t r = 0; r < roomNum; ++r){
        cout << "//castle room " << r << '\n';

        for (uint32_t i = 0; i < roomLength; i++){
            for (uint32_t j = 0; j < roomLength; j++){
                cout << map[r][i * roomLength + j];
            }
            cout << '\n';
        }
        }
    }

    // cout list;
    else{
        cout << "Path taken:\n";

        for (const auto &step : path){
            cout << '(' << step.room << ',' << step.row << ',' 
            << step.col << ',' << map[step.room][step.row * roomLength + step.col] << ")\n";
        }
    }
};

int main(int argc, char *argv[]){
    ios_base::sync_with_stdio(false); // speed up I/O;
    Rescue Marco;

    // store routing schemes & output type;
    ProgramOptions options;

    // get mode;
    Marco.getMode(argc, argv, options);

    // read in map;
    Marco.readMap();

    // according to the routing schemes find the path & change values;
    Marco.routSchemes(options);

    // cout out the map;
    if (solution) Marco.outputMap(options);
    return 0;
}