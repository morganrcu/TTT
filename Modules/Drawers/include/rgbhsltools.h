/*
 * rgbhsltools.h
 *
 *  Created on: Dec 11, 2013
 *      Author: morgan
 */

#ifndef RGBHSLTOOLS_H_
#define RGBHSLTOOLS_H_
unsigned int HSLtoRGB(const unsigned int& h, const unsigned int& s, const unsigned int& l)
{
	unsigned int r = 0;
	unsigned int g = 0;
	unsigned int b = 0;

	float L = ((float)l)/100;
	float S = ((float)s)/100;
	float H = ((float)h)/360;

	if(s == 0)
	{
		r = l;
		g = l;
		b = l;
	}
	else
	{
		float temp1 = 0;
		if(L < .50)
		{
			temp1 = L*(1 + S);
		}
		else
		{
			temp1 = L + S - (L*S);
		}

		float temp2 = 2*L - temp1;

		float temp3 = 0;
		for(int i = 0 ; i < 3 ; i++)
		{
			switch(i)
			{
			case 0: // red
				{
					temp3 = H + .33333f;
					if(temp3 > 1)
						temp3 -= 1;
					HSLtoRGB_Subfunction(r,temp1,temp2,temp3);
					break;
				}
			case 1: // green
				{
					temp3 = H;
					HSLtoRGB_Subfunction(g,temp1,temp2,temp3);
					break;
				}
			case 2: // blue
				{
					temp3 = H - .33333f;
					if(temp3 < 0)
						temp3 += 1;
					HSLtoRGB_Subfunction(b,temp1,temp2,temp3);
					break;
				}
			default:
				{

				}
			}
		}
	}
	r = (unsigned int)((((float)r)/100)*255);
	g = (unsigned int)((((float)g)/100)*255);
	b = (unsigned int)((((float)b)/100)*255);
	return RGB(r,g,b);
}


#endif /* RGBHSLTOOLS_H_ */
