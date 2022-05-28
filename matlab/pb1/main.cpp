///------------------------------ DEBUT EN TETE ----------------------------------------------///
/// NOM :               PROBLÈME 1 - APP G1D - Implémentation C++                    	      	///
///                                                                                  	      	///
/// AUTEURS :     Théodore Prévot, Saly Yang, Jean Dulong De Rosnay                  	      	///
///               Jane Siewe, Benjamin Dutilleul, Maximilien Lavergne                	      	///
///                                                                                  	      	///
/// VERSION :     1.1 Création du fichier matlab                                              ///
///                                                                                           ///
/// HISTORIQUE :  Aucun                                                                       ///
///                                                                                           ///
/// PARAMÈTRES :                                                                              ///
///	    * G = 30                        constante     Gain de l'amplification du signal	      ///
///     * PSPL = 80                     constante     Valeur minimum pour un bruit            ///
///     * S = -48                       constante     Sensibilité du micro                    ///
/// SORTIES :                                                                                 ///
///    * Durée du bruit entier          entier        Durée de chaque bruit detecté           ///
///    * Puissance moyenne              entier        Puissance moyenne du bruit en dBm       ///
///    * Puissance moyenne              entier        Puissance moyenne du bruit en mW        ///
///    * Tension RMS En V               entier        Tension RMS en V du son                 ///
///	                                                                                          ///
///    * Psilence	                      Vecteur       Puissance des silences du signal        ///
///    * Pbruit	                        Vecteur       Puissance des bruits du signal          ///
///    * Psignal                        Vecteur       Signal carré (bruit = 1, silence = -1)  ///
///                                                                                           ///
/// BIBLIOTHÈQUES :																																						///
///    * AudioFile(https://github.com/adamstark/AudioFile)                                    ///
///    * Standard Library (std)                                                               ///
/// FONCTIONS APPELEES :                                                                      ///
///	   *                                                                             					///
///	   * length                                                                               ///
///	   * log10                                                                                ///
///	   * sqrt                                                                                 ///
///                                                                                           ///
/// ALGO - REFERENCES :                                                                       ///
///   * https://www.figma.com/file/2jdpUKlbHxAkkBcXpv5S9a (schéma de l'algorithme)            ///
///-------------------------------------------------------------------------------------------///

#include <AudioFile.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

constexpr int K = 1100;
constexpr int G = 30;
constexpr int PSPL = 80;
constexpr int S = -48;
constexpr int Pdbm = PSPL + G + S - 64;

using sclock = std::chrono::steady_clock;

double powerMean(const std::vector<double> &data)
{
	double sum = 0;
	for (int i = 0; i < data.size(); i++)
	{
		sum += data[i] * data[i];
	}
	return sum / data.size();
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <audiofile.wav>" << std::endl;
		return 1;
	}

	sclock::time_point begin = sclock::now();
	std::cout.precision(5);
	AudioFile<double> audioFile;
	audioFile.load(argv[1]);
	audioFile.printSummary();

	int N = audioFile.getNumSamplesPerChannel();
	int FS = audioFile.getSampleRate();

	double te = 1 / FS;
	std::vector<double> y = audioFile.samples[0];
	std::vector<double> P(N, 0);
	std::vector<double> PSilence(N, 0);
	std::vector<double> PBruit(N, 0);
	std::vector<int> SBruit(N, 0);

	// calcul de la puissance instantanee
	for (int n = K + 1; n < N - K; n++)
	{
		double mean = 0;
		for (int i = n - K; i < n + K; i++)
			mean += y[i] * y[i];
		mean /= K * 2;
		P[n] = 10 * log10(mean / pow(10, -3));
	}
	std::cout << "Puissance instantanee calculee\n"
						<< std::endl;

	int debBruit = 0;
	int debSilence = 0;

	for (int n = 0; n < N; n++)
	{
		// Si on depasse le seuil de bruit actuel
		if (P[n] > Pdbm)
		{

			// Si jamais c'est un nouveau bruit on definit l'emplacement du debut du bruit
			if (debBruit == 0)
				debBruit = n;

			PBruit[n] = P[n]; // On attribue la puissance qui correspond au bruit
			debSilence = 0;		// On reset le debut de silence à 0 car on est en stade de 'bruit'
		}
		else
		{ // S'il n'y a pas de bruit c'est que c'est un silence

			// Si jamais c'est un nouveau silence on definit l'emplacement du debut du silence
			if (debSilence == 0)
			{
				debSilence = n;

				// Si jamais on à detecte on passe d'un silence à un bruit
				if (n - debBruit > FS)
				{
					std::cout << "Duree du bruit du son: " << ((double)(n - debBruit) / (double)FS) << std::endl;
					std::vector<double> subdataY(y.begin() + debBruit, y.begin() + n);
					std::vector<double> subdataP(P.begin() + debBruit, P.begin() + n);
					double PMoydbm = powerMean(subdataP); // moyenne de la puissance du debut à la fin d’un bruit, au carre
					double PMoymw = powerMean(subdataY);	// moyenne du signal d’origine du debut à la fin d’un bruit, au carre
					double ValEff = sqrt(PMoymw);					// tension RMS ou valeur efficace du signal
					std::cout << "Puissance moyenne en dBm du son: " << PMoydbm << std::endl;
					std::cout << "Puissance moyenne en mW du son: " << PMoymw << std::endl;
					std::cout << "Tension RMS en V du son: " << ValEff << "\n"
										<< std::endl;
				}
			}

			PSilence[n] = P[n]; // On attribue la puissance qui correspond au silence
			debBruit = 0;				// On reset le debut de bruit à 0 car on est en stade de 'silence'
		}

		// Si on a detecte du bruit, que ca fait plus de 1s et que
		// l'intervalle de detection du bruit n'as pas ete rempli on definit
		// le signal bruit à 1 depuis le debut de la detection jusqu'à
		// maintenant
		if (P[n] > Pdbm && n - debBruit > FS && SBruit[debBruit] == 0)
		{
			for (int m = debBruit; m < n; m++)
				SBruit[m] = 1;

			// Si on a detecte du bruit, que ca fait plus de 1s  et que
			// l'intervalle de detection à dejà ete rempli on paramètre juste
			// pour la valeur actuelle
		}
		else if (P[n] > Pdbm && n - debBruit > FS)
			SBruit[n] = 1;

		// Si on a detecte du silence, que ca fait plus de 1s et que
		// l'intervalle de detection du silence n'as pas ete rempli on definit
		// le signal silence à 1 depuis le debut de la detection jusqu'�
		// maintenant
		if (P[n] < Pdbm && n - debSilence > 0.5 * FS && SBruit[debSilence] == 0)
		{

			for (int m = debSilence; m < n; m++)
				SBruit[m] = -1;
		}
		// Si on a detecte du silence, que ca fait plus de 1s  et que
		// l'intervalle de detection à dejà ete rempli on paramètre juste
		// pour la valeur actuelle
		else if (P[n] < Pdbm && n - debSilence > 0.5 * FS)
			SBruit[n] = -1;
	}

	std::cout << "Elapsed time: " << (double)(std::chrono::duration_cast<std::chrono::milliseconds>(sclock::now() - begin).count() / 1000.0) << "s" << std::endl;
}