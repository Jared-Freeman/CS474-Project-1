 /*
  * @description: 
  * This program implements altering image quantization on the supplied .PGM image.
  *
  * @author: 
  * Jared Freeman
  *
  */
  
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

void ProcessImage(std::string path);

int main(int argc, char** argv)
{
  //Extract args into vector
  std::vector<std::string> args;
  for(int i=1; i < argc; i++)
  {
  std::string next_element(argv[i]);  
  args.push_back(next_element);
  }
  
	//Extract image paths
  std::vector<std::string> imagePaths = ExtractArgs("-image", args);
  
  //Discard any malformed paths and inform user (possibly when iterating)
  
  //Process each image
  for(int i=0; i < imagePaths.size(); i++)
  {
    std::cout << "_________________________\n";
    std::cout << "image " << i << ": \"" << imagePaths[i] << "\"\n";
    
    ProcessImage(imagePaths[i]);
  }
  
  
  return 0;
}

void ProcessImage(std::string path)
{
  std::fstream stream(path);
  if(!stream.is_open())
  {
    std::cout << "File failed to open!\n";
  }
  else
  {
    
  }
}




