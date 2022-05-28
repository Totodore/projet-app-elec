%%%------------------------------ DEBUT EN TETE ----------------------------------------------%%%
%%% NOM :               PROBLÈME 1 - APP G1D                                         	      %%%
%%%                                                                                  	      %%%
%%% AUTEURS :     Théodore Prévot, Saly Yang, Jean Dulong De Rosnay                  	      %%%
%%%               Jane Siewe, Benjamin Dutilleul, Maximilien Lavergne                	      %%%
%%%                                                                                  	      %%%
%%% VERSION :     1.1 Création du fichier matlab                                              %%%
%%%                                                                                           %%%
%%% HISTORIQUE :  Aucun                                                                       %%%
%%%                                                                                           %%%
%%% PARAMÈTRES :                                                                              %%%
%%%	    * G = 30                        constante     Gain de l'amplification du signal	      %%%
%%%     * PSPL = 80                     constante     Valeur minimum pour un bruit            %%%
%%%     * S = -48                       constante     Sensibilité du micro                    %%%
%%% SORTIES :                                                                                 %%%
%%%    * Durée du bruit entier          entier        Durée de chaque bruit detecté           %%%
%%%    * Puissance moyenne              entier        Puissance moyenne du bruit en dBm       %%%
%%%    * Puissance moyenne              entier        Puissance moyenne du bruit en mW        %%%
%%%    * Tension RMS En V               entier        Tension RMS en V du son                 %%%
%%%    * Coefficient d'autocorrel max   entier        Tension RMS en dBm du son               %%%
%%%    * Délai correspondant max        entier        Délai maximum de correspondance         %%%
%%%	                                                                                          %%%
%%%    * Psilence	                      Vecteur       Puissance des silences du signal      %%%
%%%    * Pbruit	                        Vecteur       Puissance des bruits du signal          %%%
%%%    * Psignal                        Vecteur       Signal carré (bruit = 1, silence = -1)  %%%
%%%                                                                                           %%%
%%% MODIFIEES                                                                                 %%%
%%%                                                                                           %%%
%%% FONCTIONS APPELEES :                                                                      %%%
%%%	   * audioread                                                                            %%%
%%%	   * length                                                                               %%%
%%%	   * zeros                                                                                %%%
%%%	   * log10                                                                                %%%
%%%	   * mean                                                                                 %%%
%%%	   * sqrt                                                                                 %%%
%%%	   * xcorr                                                                                %%%
%%%	   * max                                                                                  %%%
%%%                                                                                           %%%
%%% ALGO - REFERENCES :                                                                       %%%
%%%   * https://www.figma.com/file/2jdpUKlbHxAkkBcXpv5S9a (schéma de l'algorithme)            %%%                                                                               					%%%
%%%-------------------------------------------------------------------------------------------%%%

clc
close all
clear all
tic
G = 30;
PSPL = 80;
S = -48;

Pdbm = PSPL + G + S - 64;
K = 1100;

% Lecture du fichier
[y, FS] = audioread('../../TD5/Jardin02.mp3');

% Taille du vecteur signal
N = length(y);

Te = 1 / FS; % Période d'echantillonage
T = 0:Te:N / FS - Te; % Echelle temporelle avec un pas de T et une valeur max de N/FS (on soustrait T car on par de 0 et pas 1)

% On initialise le trois vecteurs de puissance instantanée à 0
% Puissance en dB
P = zeros(1, N);

% On initialise deux autres vecteurs de puissance instantannée qui
% auront une valeur attribuée seulement en cas de bruit ou de silence
Psilence = zeros(1, N);
Pbruit = zeros(1, N);

% Soit un signal carré avec 1 en cas de bruit et 0 en cas de silence
signalBruit = zeros(1, N);

% Calcul de toute la puissance instantannée
for n = K + 1:1:N - K
    P(n) = 10 * log10(mean(y(n - K:n + K).^2) / (10^ - 3));
end

%% Variables pour savoir la position du début d'un bruit ou d'un silence
debBruit = 0;
debSilence = 0;

% On parcourt toute le vecteur de puissance instantanée pour calculer
% les temps de bruit et de silence
for n = 1:1:N
    % Si on dépasse le seuil de bruit actuel
    if P(n) > Pdbm

        % Si jamais c'est un nouveau bruit on définit l'emplacement du début du bruit
        if debBruit == 0
            debBruit = n;
        end

        Pbruit(n) = P(n); % On attribue la puissance qui correspond au bruit
        debSilence = 0; % On reset le début de silence à 0 car on est en stade de 'bruit'
    else % S'il n'y a pas de bruit c'est que c'est un silence

        % Si jamais c'est un nouveau silence on définit l'emplacement du début du silence
        if debSilence == 0
            debSilence = n;

            % Si jamais on à detecté on passe d'un silence à un bruit
            if (n - debBruit > FS)
                disp("Durée du bruit du son: " + ((n - debBruit) / FS));
                PMoydbm = mean(P(debBruit:n).^2); % moyenne de la puissance du début à la fin d’un bruit, au carré
                PMoymw = mean(y(debBruit:n).^2); % moyenne du signal d’origine du début à la fin d’un bruit, au carré
                ValEff = sqrt(PMoymw(end)); % tension RMS ou valeur efficace du signal
                disp("Puissance moyenne en dBm du son: " + PMoydbm);
                disp("Puissance moyenne en mW du son: " + PMoymw);
                disp("Tension RMS en V du son: " + ValEff);
                [auto, lags] = xcorr(y);
                auto = auto .* 1 / length(y);
                [m, index] = max(auto(debBruit:n));
                delai = lags(index + length(y)) * Te;
                disp("Coefficient d'autocorrélation du son: " + m);
                disp("Délai correspondant du son: " + delai);
                disp("    ");
            end

        end

        Psilence(n) = P(n); % On attribue la puissance qui correspond au silence
        debBruit = 0; % On reset le début de bruit à 0 car on est en stade de 'silence'
    end

    % Si on a detecté du bruit, que ca fait plus de 1s et que
    % l'intervalle de détection du bruit n'as pas été rempli on définit
    % le signal bruit à 1 depuis le début de la detection jusqu'�
    % maintenant
    if P(n) > Pdbm && n - debBruit > FS && signalBruit(debBruit) == 0

        for m = debBruit:1:n
            signalBruit(m) = 1;
        end

        % Si on a detecté du bruit, que ca fait plus de 1s  et que
        % l'intervalle de détection à déjà été rempli on paramètre juste
        % pour la valeur actuelle
    elseif P(n) > Pdbm && n - debBruit > FS
        signalBruit(n) = 1;
    end

    % Si on a detecté du silence, que ca fait plus de 1s et que
    % l'intervalle de détection du silence n'as pas été rempli on définit
    % le signal silence à 1 depuis le début de la detection jusqu'�
    % maintenant
    if P(n) < Pdbm && n - debSilence > 0.5 * FS && signalBruit(debSilence) == 0

        for m = debSilence:1:n
            signalBruit(m) = -1;
        end

        % Si on a detecté du silence, que ca fait plus de 1s  et que
        % l'intervalle de détection à déjà été rempli on paramètre juste
        % pour la valeur actuelle
    elseif P(n) < Pdbm && n - debSilence > 0.5 * FS
        signalBruit(n) = -1;
    end

end

toc
figure (1);
hold on;
plot(T, Psilence, 'g')
plot(T, Pbruit, 'r')
plot(T, signalBruit);
ylabel("Puissance (dBm)")
xlabel("Temps (s)")
title('Bruits et silences du son Jardin.mp3')
