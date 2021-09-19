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
#include <cstring>

#include "image.h"
#include "WriteImage.h"
#include "ReadImage.h"

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

void QuantizeImage(ImageType& image, int L);

int main(int argc, char** argv)
{
  //Extract args into vector
  std::vector<std::string> args;
  for(int i=1; i < argc; i++)
  {
  std::string next_element(argv[i]);  
  args.push_back(next_element);
  }
  
	//Storage
  std::vector<std::string> imagePaths = ExtractArgs("-in", args);
  std::vector<std::string> outputPaths = ExtractArgs("-out", args);
  if(outputPaths.size() > 0)
  {
    if(outputPaths[0][outputPaths[0].length()] != '/')
      {
        outputPaths[0] = outputPaths[0] + "/";
        //std::cout << outputPaths[0] << "\n";
      }
  }
  if(outputPaths.size() > 1)
  {
    std::cout << "Can only specify one output path. All files will be saved in the first output directory.\n";
  }
    
  //Process each image
  for(int i=0; i < imagePaths.size(); i++)
  {
    std::cout << "_________________________\n";
    std::cout << "image " << i << ": \"" << imagePaths[i] << "\"\n";
    
    ImageType next_image;

    char *cstr = new char[imagePaths[i].length() + 1];
    strcpy(cstr, imagePaths[i].c_str());

    std::readImage(cstr, next_image);

    int N, M, Q;
    next_image.getImageInfo(N, M, Q);
    //std::cout << N << ", " << M << ", " << Q << std::endl;

    for(int k=2; k < 256; k *= 4)
    {
      ImageType image_copy(next_image);
      QuantizeImage(image_copy, k);

      //determine original file name from path string
      std::string original_filename = "";
      for(int l = imagePaths[i].length(); imagePaths[i][l] != '/' && l >= 0; l--)
      {
        if(imagePaths[i][l] != '/')
        {
          std::string temp;
          temp += imagePaths[i][l];
          original_filename.insert(0, temp);
        }
      }

      std::string out_file = outputPaths[0] + original_filename + "_L_" + std::to_string(k) + ".pgm";
      char *cstr = new char[out_file.length() + 1];
      strcpy(cstr, out_file.c_str());

      std::writeImage(cstr, image_copy);

      std::cout << " * Saved image: " << out_file << "\n";
      delete [] cstr;
    }

    delete [] cstr;

    std::cout << "\n";
  }
  
  
  return 0;
}

void QuantizeImage(ImageType& image, int L)
{
  int N, M, Q;
  image.getImageInfo(N, M, Q);

  int step = (Q + 1) / L;
  for(int i=0; i<N; i++)
  {
    for(int j=0; j<M; j++)
    {
      int b;
      image.getPixelVal(i, j, b);

      //std::cout << b << ", ";

      b /= step;
      b *= step;

      //std::cout << b << "\n";

      image.setPixelVal(i, j, b);
    }
  }
}




