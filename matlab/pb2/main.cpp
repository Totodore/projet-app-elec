#include <AudioFile.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <chrono>
using sclock = std::chrono::steady_clock;

void split(std::vector<double> *y)
{
	int start, end;
	for (int i = 0; i < y->size(); i++)
	{
		if ((*y)[i] > 0.01)
		{
			start = i;
			break;
		}
	}
	for (size_t i = y->size() - 1; i >= 0; i--)
	{
		if ((*y)[i] > 0.01)
		{
			end = (int)i;
			break;
		}
	}
	*y = std::vector<double>(y->begin() + start, y->end() - end);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <audiofile.wav>" << std::endl;
		return 1;
	}

	std::cout.precision(5);
	AudioFile<double> audioFile;
	audioFile.load(argv[1]);
	audioFile.printSummary();

	int N = audioFile.getNumSamplesPerChannel();
	int FS = audioFile.getSampleRate();

	double te = 1 / FS;
	std::vector<double> y = audioFile.samples[0];

	int start, end;
	for (int i = 0; i < y.size(); i++)
	{
		if (y[i] > 0.01)
		{
			start = i;
			break;
		}
	}
	for (int i = (int)y.size() - 1; i >= 0; i--)
	{

		if (y[i] > 0.01)
		{
			end = i;
			break;
		}
	}
	y = std::vector(y.begin() + start, y.begin() + end);
	const auto [min, max] = std::minmax_element(y.begin(), y.end());

	std::vector<int> peaks;
	double treshold = *max - (*max - *min) * 0.3;
	std::cout << "Treshold: " << treshold << std::endl;
	for (int i = 0; i < y.size(); i++)
	{
		if (i > y.size() - 2 || i < 1)
			continue;
		if (y[i] > treshold && y[i + 1] < y[i] && y[i - 1] < y[i])
		{
			peaks.push_back(i);
			// std::cout << "Peak at: " << (double)((double)i / (double)FS) << std::endl;
		}
	}
	double delta = 0;
	for (int i = 0; i < peaks.size(); i++)
	{
		if (i > peaks.size() - 2)
			continue;
		delta += (double)(peaks[i + 1] - peaks[i]);
		delta /= 2;
	}
	std::cout << "Frequence cardiaque: " << (double)(60 / (double)(delta / FS)) << " bpm" << std::endl;
	return 0;
}