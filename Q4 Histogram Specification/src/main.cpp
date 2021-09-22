 /*
  * @description: 
  * This program implements histogram specification on the supplied .PGM image.
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
#include <map>
#include <math.h>

#include "image.h"
#include "WriteImage.h"
#include "ReadImage.h"

#include "freeman_arg_parse.h" //A small utility I wrote for extracting command line args.

void QuantizeImage(ImageType& image, int L);
void SpecifyHistogram(ImageType& image, ImageType& histogram_reference);

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
  std::vector<std::string> specifiedPaths = ExtractArgs("-spec", args);
  std::vector<std::string> outputPaths = ExtractArgs("-out", args);
  if(outputPaths.size() > 0 && outputPaths[0].length() > 0)
  {
    if(outputPaths[0][outputPaths[0].length()-1] != '/')
      {
        outputPaths[0] = outputPaths[0] + "/";
        //std::cout << outputPaths[0] << "\n";
      }
  }
  if(outputPaths.size() > 1)
  {
    std::cout << "Can only specify one output path. Output will be saved in the first output directory.\n";
  }  
  if(specifiedPaths.size() != imagePaths.size())
  {
    std::cout << "ERROR! Number of input images and specification images must match.\n";
    return 0;
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
    
    ImageType spec_image;
    char *cstr2 = new char[specifiedPaths[i].length() + 1];
    strcpy(cstr2, specifiedPaths[i].c_str());
    std::readImage(cstr2, spec_image);    

    ImageType image_copy(next_image);
    SpecifyHistogram(image_copy, spec_image); //test

    { //Write to file scope

      //determine original file name from path string
      std::string original_filename = "";
      for(int l = imagePaths[i].length(); imagePaths[i][l] != '/' && l >= 0; l--)
      {
        if(imagePaths[i][l] != '/')
        {
          //std::cout << imagePaths[i][l] << std::endl;

          std::string temp;
          temp += imagePaths[i][l];
          original_filename.insert(0, temp);
        }
        //throw out extension
        if(imagePaths[i][l] == '.')
        {
          original_filename = "";
        }
      }      
      
      //determine spec original file name from path string. 
      //Yes this is duplicate code... sorry
      std::string original_filename_s = "";
      for(int l = specifiedPaths[i].length(); specifiedPaths[i][l] != '/' && l >= 0; l--)
      {
        if(specifiedPaths[i][l] != '/')
        {
          //std::cout << imagePaths[i][l] << std::endl;

          std::string temp;
          temp += specifiedPaths[i][l];
          original_filename_s.insert(0, temp);
        }
        //throw out extension
        if(specifiedPaths[i][l] == '.')
        {
          original_filename_s = "";
        }
      }

      std::string out_file = outputPaths[0] + original_filename + "_specified_" 
        + original_filename_s + ".pgm";
      char *cstr = new char[out_file.length() + 1];
      strcpy(cstr, out_file.c_str());

      std::writeImage(cstr, image_copy);

      std::cout << " * Saved image: " << out_file << "\n";
      delete [] cstr;
    } //end Write to file

    delete [] cstr;
    delete [] cstr2;

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

      b /= step;
      b *= step;

      image.setPixelVal(i, j, b);
    }
  }
}

void SpecifyHistogram(ImageType& image, ImageType& histogram_reference)
{

  std::map<int, float> specified_histogram;
  std::map<int, float> image_histogram;
  std::map<int, float> transform;
  float s_num_pixels, i_num_pixels;

  int N_s, M_s, Q_s;
  histogram_reference.getImageInfo(N_s, M_s, Q_s);
  s_num_pixels = N_s * M_s;

  int N_i, M_i, Q_i;
  histogram_reference.getImageInfo(N_i, M_i, Q_i);
  i_num_pixels = N_i * M_i;

  //just in case
  if(Q_i != Q_s) return;

  int s_maxPixelValue = 0;
  int i_maxPixelValue = 0;

  for(int i=0; i<256; i++)
  {
    specified_histogram[i] = 0.0f;
  }
  // Build specified histogram, gather value counts
  for (int i = 0; i < N_s; ++i)
  {
    for (int j = 0; j < M_s; ++j)
    {
      int p;
      histogram_reference.getPixelVal(i, j, p);

      if (p > s_maxPixelValue)
        s_maxPixelValue = p;
      specified_histogram[p]++;
    }
  }  
  
  for(int i=0; i<256; i++)
  {
    image_histogram[i] = 0.0f;
  }
  // Build image histogram, gather value counts
  for (int i = 0; i < N_s; ++i)
  {
    for (int j = 0; j < M_s; ++j)
    {
      int p;
      image.getPixelVal(i, j, p);

      if (p > i_maxPixelValue)
        i_maxPixelValue = p;
      image_histogram[p]++;
    }
  }


  // Compute frequencies specified
  for (auto iter : specified_histogram)
  {
    specified_histogram[iter.first] = float(iter.second / s_num_pixels);
  }
  // Compute frequencies image
  for (auto iter : image_histogram)
  {
    image_histogram[iter.first] = float(iter.second / i_num_pixels);
  }



  // Compute mapped int vals specified
  float cumulProb = 0.0;
  for (auto iter : specified_histogram)
  {
    specified_histogram[iter.first] = float(floor(s_maxPixelValue * (iter.second + cumulProb)));
    cumulProb += iter.second;
  }
  // Compute mapped int vals image
  cumulProb = 0.0;
  for (auto iter : image_histogram)
  {
    image_histogram[iter.first] = float(floor(i_maxPixelValue * (iter.second + cumulProb)));
    cumulProb += iter.second;
  }

  int pixel = 0;

  int test = 0;
  for(auto i_it : image_histogram)
  {
    float record = 0;
    bool done = false;
    for(auto s_it : specified_histogram)
    {
      if(i_it.second == s_it.second)
      {
        record = s_it.first;
      }
      else if(i_it.second > s_it.second)
      {
        record = s_it.first;
      }
      else
      {
        transform[test++] = record;
        done = true;
        break;
      }
    }
    if(!done) //reached if no greater value found in search
    {
      transform[test++] = record;
    }
  }

  // Apply transformation
  for (int i = 0; i < N_i; ++i)
  {   
    for (int j = 0; j < M_i; ++j)
    {
        image.getPixelVal(i, j, pixel);
        image.setPixelVal(i, j, transform[pixel]);
    }
  }
  
}


