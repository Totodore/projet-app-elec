%%%------------------------------ DEBUT EN TETE ----------------------------------------------%%%
%%% NOM :               PROBLÈME 4 - APP G1D                                         	        %%%
%%%                                                                                  	        %%%
%%% AUTEURS :     Théodore Prévot, Saly Yang, Jean Dulong De Rosnay                  	        %%%
%%%               Jane Siewe, Benjamin Dutilleul, Maximilien Lavergne                	        %%%
%%%                                                                                  	        %%%
%%% VERSION :     1.1 Création du fichier matlab                                              %%%
%%%                                                                                           %%%
%%% HISTORIQUE :  Aucun                                                                       %%%
%%%                                                                                           %%%
%%% PARAMÈTRES :                                                                              %%%
%%%	    * G = 30                        constante     Gain de l'amplification du signal	      %%%
%%%     * PSPL = 74                     constante     Valeur minimum pour un bruit            %%%
%%%     * S = -55                       constante     Sensibilité du micro                    %%%
%%% SORTIES :                                                                                 %%%
%%%    * Durée du bruit entier          entier        Durée de chaque bruit detecté           %%%
%%%    * Puissance moyenne              entier        Puissance moyenne du bruit en dBm       %%%
%%%    * Puissance moyenne              entier        Puissance moyenne du bruit en mW        %%%
%%%    * Tension RMS En V               entier        Tension RMS en V du son                 %%%
%%%    * Coefficient d'autocorrel max   entier        Tension RMS en dBm du son               %%%
%%%    * Délai correspondant max        entier        Délai maximum de correspondance         %%%
%%%	                                                                                          %%%
%%%    * Psilence	                      Vecteur       Puissance des silences du signal        %%%
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
%%%	   * max                                                                                  %%%
%%%	   * filter                                                                               %%%
%%%                                                                                           %%%
%%% ALGO - REFERENCES :                                                                       %%%
%%%   * https://www.figma.com/file/2jdpUKlbHxAkkBcXpv5S9a (schéma de l'algorithme)            %%%                                                                               					%%%
%%%-------------------------------------------------------------------------------------------%%%

function [T, Psilence, Pbruit, signalBruit] = Probleme4(audio)
    G = 30;
    PSPL = 74;
    S = -55;

    Pdbm = PSPL + G + S - 64;
    K = 1100;
    % Lecture du fichier
    [y, FS] = audioread(audio);

    load('Probleme4.mat'); % On charge le filtre en mémoire pour l'utiliser dans le code (variable h)

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
                    PMoydbm = mean(P(debBruit:n).^2); % moyenne de la puissance du début à la fin d’un bruit, au carré
                    PMoymw = mean(y(debBruit:n).^2); % moyenne du signal d’origine du début à la fin d’un bruit, au carré

                    bruit = y(debBruit:n);
                    Pb = bruit .* bruit; % % Puissance instantannée
                    Pmoy = 1 / (2 * (n - debBruit) + 1) * sum(Pb); % % Puissance moyenne

                    filtered2 = filter(h, 1, bruit).^2; % Notre signal filtré qui correspond au bruit, au carré

                    filteredPm = 1 / (2 * (n - debBruit) + 1) * sum(filtered2); % Puissance moyenne du signal filtré
                    ratio = filteredPm / Pmoy; % % Pourcentage du bruit dont les fréquences sont compris entre 2KHz

                    disp("   ");
                    disp("Bruit détecté:");
                    fprintf("Durée du bruit %4.3f\n", (n - debBruit) / FS);

                    if (ratio > 0.2)
                        fprintf("C'est un bruit fort avec %4.3f %% d'aigues (>2KHz)\n", ratio * 100);
                    else
                        fprintf("C'est un bruit faible avec %4.3f %% d'aigues (>2KHz)\n", ratio * 100);
                    end

                    fprintf("Puissance moyenne du bruit en dBm : %4.3f\n", PMoydbm);
                    fprintf("Puissance moyenne du bruit en mW : %4.3f\n", PMoymw);
                    disp("   ");
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

end
