//--------------------------------------------------------------------------------
// This program is useful to download UniMi lectures from www.ariel.unimi.it
//
// Type in the LecDownloaderInput.dat file the following data, separated by space
// m3u8 lecture link;
//
// Course name (AM2 for mathematical analysis, Qua1 for Quantum Physics 1 etc.)
// Lecture date in YYYYMMDD format
// Lecture number (LecXX for lessons, TutorXX for tutor lectures, EsXX for 
// pratical lessons) 
//
//--------------------------------------------------------------------------------
// Author: Rossi Alessandro Matteo          Date: 2021/10
// GitHub: https://github.com/Rossi-AM
//--------------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>


//This function counts the lines of the LecDownloaderInput.dat file, in order to understand how many lectures the program has to download

int LineCounter();

//This is a more technical function. In order to perform commands from the cmd line system() has to be used. Its argument is a composition of external commands and data provided by the user. This functions combines all of the different strings in a single one.

std::string ConcatenateFourStrings(std::string &s1, std::string &s2, std::string &s3, std::string &s4);



int main(int argc, char **argv)
{
    const std::string inputFile = "LecDownloaderInput.dat"; //Input file name
    std::string Link, Course, LecDate, LecNumber; //Input file data

    //LecDownloaderInput.dat gets opened

    std::fstream Scan;
    Scan.open("LecDownloaderInput.dat",std::ios::in);
    if(!Scan.good())
    {
        std::cout << "ERROR: a problem occured while opening LecDownloaderInput.dat2" << std::endl;
    }

    //A for loop is performed on the opened file and the command gets built in Command_Argument_String. However system() argument needs to be a char, so the string gets converted in a char using strcpy() before encountering system()
   
    for (int i = 0; i <= LineCounter(); i++)
        {
            Scan >> Link >> Course >> LecDate >> LecNumber;
            std::string Command_Argument_String = ConcatenateFourStrings(Link, Course, LecDate, LecNumber);

            int char_size = Command_Argument_String.length();
            char Command_Argument_Char[char_size + 1];
            strcpy(Command_Argument_Char, Command_Argument_String.c_str());

            system(Command_Argument_Char);
    }
    
    return 0;
}


//--------------------------------------------------------------------------------

int LineCounter()
{
    int counter = 0;
    std::string temp;

    std::fstream Scan;
    Scan.open("LecDownloaderInput.dat",std::ios::in);
    if(!Scan.good())
    {
        std::cout << "ERROR: a problem occured while opening LecDownloaderInput.dat" << std::endl;
        return 1;
    }

    for (;; counter++)
    {
        Scan >> temp;

        if(Scan.eof())
            break;
    }
    Scan.close();
    return counter/4;
}



std::string ConcatenateFourStrings(std::string &s1, std::string &s2, std::string &s3, std::string &s4)
{
    std::string cmd1 = "ffmpeg -protocol_whitelist file,http,https,tcp,tls,crypto -i \"";
    std::string cmd2 = "\" -c copy ";
    std::string cmd3 = "_";
    std::string cmd4 = ".mp4";

    return cmd1 + s1 + cmd2 + s2 + cmd3 + s3 + cmd3 + s4 + cmd4;
}
    
//--------------------------------------------------------------------------------