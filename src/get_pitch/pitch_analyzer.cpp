/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;

/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {

  		/// \TODO Compute the autocorrelation r[l] 
      /// \FET Hem computat la autocorrelació fent ús de la fórmula donada al enunciat: r[l] = sum( x[n] x[n+l] ) desde n=0 a n=N-l
      for (unsigned int n = 0; n < x.size()-l; n++) {
          r[l] += x[n]*x[n+l];
        }
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type) {
    case HAMMING:
      /// \TODO Implement the Hamming window
      /// \FET Hem implementat la Finestra de Hamming
      for (unsigned int n=0; n < frameLen; n++){
        window[n] = 0.54-0.46*cos(2*3.1416*n/frameLen);
      }

      break;
    case RECT:
    default:
      window.assign(frameLen, 1);
    }
  }


  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.  - FALTA!
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.
    /// \FET Hem determinat els llindars pels quals decidirem si es veu o no-veu.
    bool unvoiced = true;
    if (rmaxnorm > umaxnorm || r1norm>0.97){
      unvoiced = false;
    }

    if (pot<-20 ){
      unvoiced = true;
    }

    return unvoiced;
  }

  float PitchAnalyzer::compute_pitch(vector<float> & x) const {
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    /// \TODO  
	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.

  /// \FET
  /// Hem localitzat el maxim de l'autocorrelació comparant per cada iteració si el valor actual és més gran que el màxim registrat, en cas que ho sigui, aquest passarà a ser el valor màxim registrat

    //Compute correlation
    autocorrelation(x, r);

    vector<float>::const_iterator iR = r.begin(), iRMax = iR;

    for(iR = iRMax =  r.begin() + npitch_min; iR < r.begin() + npitch_max; iR++){  //r.begin para empezar por el primer valor de la autocorrelacion
        if(*iR > * iRMax) {
          iRMax = iR;             
      }
    }
    unsigned int lag = iRMax - r.begin(); 

    float pot = 10 * log10(r[0]);

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 1
    if (r[0] > 0.0F)
      cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
#endif
    
    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0]))
      return 0;
    else
      return (float) samplingFreq/(float) lag;
  }
}

