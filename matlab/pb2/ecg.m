%%%------------------------------ DEBUT EN TETE ----------------------------------------------%%%
%%% NOM :               PROBLÈME 2 - APP G1D                                         	        %%%
%%%                                                                                  	        %%%
%%% AUTEURS :     Théodore Prévot, Saly Yang, Jean Dulong De Rosnay                  	        %%%
%%%               Jane Siewe, Benjamin Dutilleul, Maximilien Lavergne                	        %%%
%%%                                                                                  	        %%%
%%% VERSION :     1.1 Création du fichier matlab                                              %%%
%%%                                                                                           %%%
%%% HISTORIQUE :  Aucun                                                                       %%%
%%%                                                                                           %%%
%%% PARAMÈTRES :                                                                              %%%
%%%	    * file          String     		Fichier du signal à étudier	      											%%%
%%% SORTIES :                                                                                 %%%
%%%    * f1						  Scalaire			Battement cardiaque déterminé par analyse temporelle    %%%
%%%    * f2						  Scalaire			Battement cardiaque déterminé par analyse fréquentielle %%%
%%%                                                                                           %%%
%%% MODIFIEES                                                                                 %%%
%%%                                                                                           %%%
%%% FONCTIONS APPELEES :                                                                      %%%
%%%	   * audioread                                                                            %%%
%%%	   * length                                                                               %%%
%%%	   * zeros                                                                                %%%
%%%	   * find                                                                                 %%%
%%%	   * detrend                                                                              %%%
%%%	   * findpeaks                                                                            %%%
%%%	   * fft                                                                                  %%%
%%%                                                                                           %%%
%%% ALGO - REFERENCES :                                                                       %%%
%%%   * https://www.figma.com/file/5jeJAnsjFMIXm6iUVQw61A  												            %%%                                                                               					%%%
%%%-------------------------------------------------------------------------------------------%%%

function [f1, f2] = ecg(file, show)
    % Chargement du signal depuis le fichier
    [s, Fs] = audioread(file); % Charge un fichier, y est un tableau qui contient les données, Fs est la fréquence d'echantillonage

    %% Caractéristiques temporelles et mise en forme

    % On supprime les toutes petites variations en début et en fin de signal
    y = s(find(s > 0.01, 1, 'first'):find(s > 0.01, 1, 'last'));
    y = detrend(y); % Filtre les variations polynomiales du signal
    N = length(y); % Taille du tableau de données
    Ts = 1 / Fs; % Calcule la période d'échantillonage
    t = (0:N - 1) * Ts; % Vecteur temps du signal

    %% Algorithme 1 : Détection des pics

    % Recherche des pics et calcul du rythme cardiaque
    % Cherche les pics et leurs positions (un point est un pic lorsqu'il est supérieur à la moitié du signal)
    [pks, locs] = findpeaks(y, 'MinPeakProminence', (max(y) - min(y)) * 1/2);
    % Calcule la periode moyenne
    T = mean(diff(locs) * Ts);
    f1 = 60 / T; % Calcule le rythme cardiaque

    if show
        % Plot le signal pré-traité ainsi que la position des pics
        figure;
        subplot(2, 1, 1);
        plot(t, y) % Plot le signal
        grid
        xlabel('Temps (s)');
        ylabel('Amplitude');
        hold on
        plot(t(locs), pks, 'ro') % Plot la position des pics
				title("Détection des pics du signal")
    end

    %% Algorithme 2 : Fréquence du pic maximum de la FFT dans une fenêtre de [0.5Hz, 3.5Hz]

    Y = fft(y); % FFT du signal
    f = (0:length(Y) - 1) * Fs / length(Y);

    fz = 0.5:Fs / length(Y):3.5;
    idx1 = find(f == 0.5);
    idx2 = find(f == 3.5);
    Z = Y(idx1:idx2);

    if show
        subplot(2, 1, 2);
        plot(fz, real(Z));
        xlabel('f (Hz)')
        ylabel('|Y(f)|')
        title("FFT du signal entre [0.5Hz et 3.5Hz]");
    end

    [val, indice] = max(real(Z));
    freq = fz(indice);
    f2 = freq * 60;
end
