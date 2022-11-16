/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Estimator 

Usage:
    get_pitch [options] <input-wav> <output-txt>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project
    -m REAL, --umaxnorm=REAL  Umbral del máximo de la autocorrelación [default: 0.4]


Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the estimation:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";


//median filter
void median_filter(vector<float> &pitches){
  vector<float> sorted = pitches;
  vector<float> sorting = pitches;
  float a;
 printf("\nholaaaaaaaaaaaa\n%i\n",pitches.size());

  for (unsigned int i = 1; i < pitches.size() - 1; i++)
  {
   
    sorting[0] = pitches[i - 1];
    sorting[1] = pitches[i];
    sorting[2] = pitches[i + 1];
    /*Aumentamos la dimensión de la ventana para comprobar su efecto*/
    //Se ha probado con cada uno
    //sorting[3] = pitches[i + 2]; //l=4
    //sorting[4] = pitches[i + 3];//L=5
    //sorting[5] = pitches[i + 4];//L=6
    
    
    
    for (int j = 0; j < 2; j++)
    {
      for (int k = 0; k < 2; k++)
      {
        if (sorting[k] > sorting[k + 1])
        {
          a = sorting[k + 1];
          sorting[k + 1] = sorting[k];
          sorting[k] = a;
        }
      }
    }
    sorted[i] = sorting[1];
  }
  pitches = sorted;
}

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
  /// \FET Hem afegit els 3 umbrals que hem definit per poderlos passar com a paràmetres

    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float umaxnorm = std::stof(args["--umaxnorm"].asString());

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::RECT, 50, 500, umaxnorm);


  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  
  ///\FET hem implementat el métode de pre-processat clipping-centre
  
  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  float alfa=0.006;

  //center-clipping
  for (iX = x.begin(); iX  < x.end(); iX++ ) {
    if (*iX < alfa && *iX > -alfa){ 
      *iX = 0;
    }
  }

  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }
  

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

  /// \FET Hem generat un filtre de mediana No Recursiu
  /// Es important que no sigui Recursiu!

  median_filter(f0);

  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
