//
//  fft.h
//  Cosmos
//
//  Created by 川口大志 on 2014/04/19.
//
//

#ifndef __Cosmos__fft__
#define __Cosmos__fft__

class fft {
    
public:
    
	fft();
	~fft();
    
	/* Calculate the power spectrum */
	void powerSpectrum(int start, int half, float *data, int windowSize,float *magnitude,float *phase, float *power, float *avg_power);
	/* ... the inverse */
	void inversePowerSpectrum(int start, int half, int windowSize, float *finalOut,float *magnitude,float *phase);
    
    
};

#endif /* defined(__Cosmos__fft__) */
