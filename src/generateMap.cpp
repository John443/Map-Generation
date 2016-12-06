#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct Position
{
	double latitude;
	double longitude;
};

int M[1000][2000];

void SplitString(const string & s, vector<string> & v, const string & c)
{
	v.clear();
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

int main()
{
	//Read File
	string path = "090401.csv";
	FILE * pFile = fopen(path.c_str(), "r");
	if (!pFile)
	{
		return 0;
	}
	string data;
	fseek(pFile, 0, SEEK_END);
	long len = ftell(pFile);
	char * pBuffer = new char[len + 1];
	fseek(pFile, 0, SEEK_SET);
	fread(pBuffer, 1, len, pFile);
	long long k = 0;
	vector<Position> pos;
	vector<string> row;
	fclose(pFile);
	pBuffer[len] = 0;
	data.assign(pBuffer, len);
	delete[] pBuffer;
	//Parsing
	string c = "\n";
	string::size_type pos1, pos2;
	pos2 = data.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		string tmp = data.substr(pos1, pos2 - pos1);
		const char * str = tmp.c_str();
		int a, f, g;
		double d, e;
		Position ptmp;
		sscanf(str, "%d%lf%lf%lf%lf%d%d", &a, &ptmp.longitude, &ptmp.latitude, &d, &e, &f, &g);
		//printf("%d %lf %lf %lf %lf %d %d\n", a, ptmp.longitude, ptmp.latitude, d, e, f, g);
		if (d >= 15)
		{
			pos.push_back(ptmp);
		}
		pos1 = pos2 + c.size();
		pos2 = data.find(c, pos1);
		/*k++;
		if (k == 100000)
			break;*/
	}
	//Graph
	Mat image = Mat(1000, 2000, CV_8U, Scalar(255));
	double longitude_center = 120.01;
	double latitude_center = 30.380;
	double cell = 0.000230;
	for (auto it = pos.begin(); it != pos.end(); it++)
	{
		int m = int((latitude_center - it->latitude) / cell);
		int n = int((it->longitude - longitude_center) / cell);
		if (n > 0 && n < 2000 && m > 0 && m < 1000 && M[m][n] >= 0)
		{
			M[m][n]++;
		}
	}
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 2000; j++)
		{
			if (M[i][j] <= 255)
				image.at<unsigned char>(i, j) = 255 - M[i][j];
			else
				image.at<unsigned char>(i, j) = 0;
			/*if (M[i][j] != 0)
				image.at<unsigned char>(i, j) = 0;*/
		}
	}

	namedWindow("Map");
	imshow("Map", image);
	imwrite("Map.bmp", image);
	waitKey(0);
	return 0;
}
