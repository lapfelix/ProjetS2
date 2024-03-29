/*
 * projet  : Projet de session S2
 * fichier : effects.cpp
 * auteur  : Eric Matte
 * date    : 17-03-2015
 */

#include "effects.h"



EffectsGenerator::EffectsGenerator(vector<RGBColor> const& colorsGrid) : colors(colorsGrid)
{
	srand (time(NULL));
}



void EffectsGenerator::gradientTransition(int transitionSize, string &os)
{
	RGBColor tempColor, stepColor;

	for (int j = 0; j < (int)colors.size() - 1; j++)
	{
		stepColor.calculateColor(0, colors[j], colors[j + 1], transitionSize - 1);

		for (int i = 0; i < transitionSize; i++)
		{
			tempColor.calculateColor(1, colors[j], stepColor, i);

			//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
            os += to_string(tempColor.getR()) + " "
				+ to_string(tempColor.getG()) + " "
				+ to_string(tempColor.getB()) + " ";
			//A remplacer par:
			//sendColorToServer(tempColor.getR(),tempColor.getG(),tempColor.getB())
		}
	}

}

vector<double> EffectsGenerator::SigmoidCalculator(double Xi, double Xf, double Yi, double Yf)
{
	vector<double> operations;
	int k = 6;// Facteur de precision
	int offset = 4;
	operations.push_back(((Yi>Yf) ? (Yi + offset) : (Yi - offset))); //new Yi
	operations.push_back(((Yi>Yf) ? (Yf - offset) : (Yf + offset))); //new Yf
	operations.push_back(k / (Xf - Xi)); //a 
	operations.push_back((Xf + Xi) / 2.0); //b

	return operations;
}

void EffectsGenerator::gradientSigmoidTransition(int transitionSize, std::string &os)
{
	RGBColor tempColor;

	for (int j = 0; j < (int)colors.size() - 1; j++)
	{
		vector<double> RedOperations = SigmoidCalculator(0, transitionSize, colors[j].getR(), colors[j + 1].getR());
		vector<double> GreenOperations = SigmoidCalculator(0, transitionSize, colors[j].getG(), colors[j + 1].getG());
		vector<double> BlueOperations = SigmoidCalculator(0, transitionSize, colors[j].getB(), colors[j + 1].getB());

		for (int i = 0; i < transitionSize; i++)
		{
			tempColor.calculateColor(RedOperations,GreenOperations,BlueOperations,i);

			//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
            os += to_string(tempColor.getR()) + " "
				+ to_string(tempColor.getG()) + " "
				+ to_string(tempColor.getB()) + " ";
			//A remplacer par:
			//sendColorToServer(tempColor.getR(),tempColor.getG(),tempColor.getB())
		}

	}

}


void EffectsGenerator::strobeTransition(int transitionTime, bool random, string &os)
{
	if (!random)
	{
		for (int j = 0; j < (int)colors.size(); j++)
		{
			for (int i = 0; i < transitionTime; i++)
			{
			//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
			os += to_string(colors[j].getR()) + " "
				+ to_string(colors[j].getG()) + " " 
			    + to_string(colors[j].getB()) + " ";
			//A remplacer par:
			//sendColorToServer(colors[j].getR(),colors[j].getG(),colors[j].getB())
			}
		}
	}
	else
	{
		int currentColor = 0;
		int randNumber = -1;
		int min = transitionTime/4;
		int max = transitionTime;

		for (int i = 0; i < transitionTime*(int)colors.size(); i++)
		{
			if (randNumber<i)
			{
				randNumber = min + (rand() % (int)(max - min + 1));
			}

			for (int j = 0; j < randNumber; j++)
			{
			
				//Envoyer les valeurs aux leds, ajouter un delais ou changer le transitionSize
				os += to_string(colors[currentColor].getR()) + " "
					+ to_string(colors[currentColor].getG()) + " " 
					+ to_string(colors[currentColor].getB()) + " ";
				//A remplacer par:
				//sendColorToServer(colors[currentColor].getR(),colors[currentColor].getG(),colors[currentColor].getB())
				i++;

				if (i >= transitionTime*(int)colors.size())
					break;
			}
			i--;
			currentColor++;
			if (currentColor>=(int)colors.size())
				currentColor=0;
		}
	}
}


////////////////////////////////////////////////
///*****************RGBColor*****************///
////////////////////////////////////////////////

//Calculs necessessaire pour la graduation
void RGBColor::calculateColor(int operation,RGBColor &color1, RGBColor &color2, int argument)
{
	switch (operation)
	{
	case 0:
		setNewColor((color2.r - color1.r) / (argument-1),
					(color2.g - color1.g) / (argument-1),
					(color2.b - color1.b) / (argument-1));
		break;
	case 1:
		setNewColor((color1.r + (color2.r*argument)),
					(color1.g + (color2.g*argument)),
					(color1.b + (color2.b*argument)));
		break;
	default:
		break;
	}
}

//Calculs necessessaire pour la graduation en sigmoid
void RGBColor::calculateColor(vector<double> RedOpp, vector<double> GreenOpp, vector<double> BlueOpp, int argument)
{
	// 0 = Yi
	// 1 = Yf
	// 2 = a
	// 3 = b
	double Red = (RedOpp[0] - RedOpp[1]) / (1 + exp(RedOpp[2] * (argument - RedOpp[3]))) + RedOpp[1];
	double Green = (GreenOpp[0] - GreenOpp[1]) / (1 + exp(GreenOpp[2] * (argument - GreenOpp[3]))) + GreenOpp[1];
	double Blue = (BlueOpp[0] - BlueOpp[1]) / (1 + exp(BlueOpp[2] * (argument - BlueOpp[3]))) + BlueOpp[1];

	setNewColor(((Red > 255) ? 255 : ((Red < 0) ? 0 : Red)),
				((Green > 255) ? 255 : ((Green < 0) ? 0 : Green)),
				((Blue > 255) ? 255 : ((Blue < 0) ? 0 : Blue)));

	/*if (y > 255)
		y = 255;
	if (y < 0)
		y = 0;�*/
}


RGBColor::RGBColor()
{
	setNewColor(0,0,0);
}
RGBColor::RGBColor(double red, double green, double blue)
{
	setNewColor(red,green,blue);
}

RGBColor::RGBColor(const char* hexColor)
{
    if (strlen(hexColor) == 7) {
        QString str1, str2, str3;
        
        str1.append(hexColor[1]);
        str1.append(hexColor[2]);
        str2.append(hexColor[3]);
        str2.append(hexColor[4]);
        str3.append(hexColor[5]);
        str3.append(hexColor[6]);
        
        bool ok;
        
            r = str1.toUInt(&ok,16);
        if (!ok)
            qDebug() << "Error with r component";
            g = str2.toUInt(&ok,16);
        if (!ok)
            qDebug() << "Error with g component";
            b = str3.toUInt(&ok,16);
        if (!ok)
            qDebug() << "Error with b component";
        qDebug() << r << g << b;
        setNewColor(r, g, b);
    }else{
        
        qDebug() << "Not the right color lenght, apparently";
        return;
    }
}
void RGBColor::setNewColor(double red, double green, double blue)
{
	r = ((red > 255) ? 255 : ((red < 0) ? 0 : red));
	g = ((green > 255) ? 255 : ((green < 0) ? 0 : green));
	b = ((blue > 255) ? 255 : ((blue < 0) ? 0 : blue));
    
    
    red = r/255;
    green = g/255;
    blue = b/255;
    //Calculs pour un xy pr�cis
    
    //correction gamma ( source: https://github.com/PhilipsHue/PhilipsHueSDK-iOS-OSX/blob/master/ApplicationDesignNotes/RGB%20to%20xy%20Color%20conversion.md )
    float newRed = (red > 0.04045f) ? pow((red + 0.055f) / (1.0f + 0.055f), 2.4f) : (red / 12.92f);
    float newGreen = (green > 0.04045f) ? pow((green + 0.055f) / (1.0f + 0.055f), 2.4f) : (green / 12.92f);
    float newBlue = (blue > 0.04045f) ? pow((blue + 0.055f) / (1.0f + 0.055f), 2.4f) : (blue / 12.92f);
    
    float X = newRed * 0.649926f + newGreen * 0.103455f + newBlue * 0.197109f;
    float Y = newRed * 0.234327f + newGreen * 0.743075f + newBlue * 0.022598f;
    float Z = newRed * 0.0000000f + newGreen * 0.053077f + newBlue * 1.035763f;
    
    x = X / (X + Y + Z);
    y = Y / (X + Y + Z);
    
    qDebug()<< x << y;
    
}

int RGBColor::getR()
{
	return (int)r;
}
int RGBColor::getG()
{
	return (int)g;
}
int RGBColor::getB()
{
    return (int)b;
}
double RGBColor::getX()
{
    return (double)x;
}
double RGBColor::getY()
{
    return (double)y;
}
const char * RGBColor::getHex()
{
    QString rstring;
    rstring.setNum((int)r,16);
    QString gstring;
    gstring.setNum((int)g,16);
    QString bstring;
    bstring.setNum((int)b,16);
    
    QString *string = new QString();
    string->append('#');
    string->append(rstring);
    string->append(gstring);
    string->append(bstring);
    qDebug() << string->toStdString().c_str();
    return string->toStdString().c_str();
}

HsbColor RGBColor::getHSB(){
    
    qDebug() << r << g << b;
    if(r == 1)
        r = r - 1e-5f;
    if(g == 1)
        g = g - 1e-5f;
    if(b == 1)
        b = b - 1e-5f;
    qDebug() << r << g << b;
    
    HsbColor result;
    
    float K = 0.f;
    
    if (g < b)
    {
        std::swap(g, b);
        
        K = -1.f;
    }
    
    if (r < g)
    {
        std::swap(r, g);
        
        K = -2.f / 6.f - K;
    }
    float chroma = r - min(g, b);
    result.hue = fabs(K + (g - b) / (6.f * chroma + 1e-20f));
    result.saturation = chroma / (r + 1e-20f);
    result.brightness = r;
    qDebug() << result.hue << result.saturation << result.brightness;
    
    return result;
}
