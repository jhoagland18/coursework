float Bin2Dec(const int bin[8]) {
    float result = 0;
    int isNegative = 0;

    if(bin[7]==1) {
            isNegative=1;
    }
	int i=0;
    for(i=0; i<7; i++) {
        if(bin[i]==1) {}
            int pow = 1;
			int j=0;
            for(j=0; j<i; j++) {
                pow=pow*2;
            }
            result += bin[i]*pow;
        }

    return isNegative==1 ? -1+(result/128): result/128;
}

void Dec2Bin(const float x, int bin[8]) {

    int isNegative = 0;

    if(x<0) {
        isNegative = 1;
    }

	int i=0;
    for(i=0; i<8; i++) {
        bin[i]=0;
    }

    if(x==-1) {
        bin[7]=1;
        return;
    }

    float scaledValue = x*128;
    float t =scaledValue;
    int intPart = (int)t;
    float fractionalPart = scaledValue - intPart;

    float value = x;
	
    for(i = 6; i >= 0; i--) {
        value = value * 2;

        if(value > 1) {
            bin[i] = 1;
            value=value-1;
        }
    }

    if(fractionalPart>.5) {
		int i=0;
            for(i=0; i<8; i++) {
                    if(bin[i]==0) {
                        bin[i]=1;
                        break;
                    } else {
                        bin[i]=0;
                    }
            }
    }


    if(isNegative==1) {
        TwosComplement(bin);
    }
}

void TwosComplement(const int input[8], int output[8]) {
    int flip = 0;

	int i=0;
    for(i=0; i<8; i++) {
            if(flip==0 && input[i]==1) {
                flip=1;
            } else if(flip==1) {
                if(input[i]==0) {
                    output[i]=1;
                } else {
                    output[i]=0;
                }
            } else {
                output[i]=input[i]; //should always be zero
            }
    }
}
