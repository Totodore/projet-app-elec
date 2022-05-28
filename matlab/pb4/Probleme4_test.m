%%%------------------------------ DEBUT EN TETE ----------------------------------------------%%%
%%% NOM :               PROBLÈME 4 - Fiche de test - APP G1D                        	        %%%
%%%                                                                                  	        %%%
%%% AUTEURS :     Théodore Prévot, Saly Yang, Jean Dulong De Rosnay                  	        %%%
%%%               Jane Siewe, Benjamin Dutilleul, Maximilien Lavergne                	        %%%
%%%                                                                                  	        %%%
%%% VERSION :     1.1 Création du fichier matlab                                              %%%
%%%                                                                                           %%%
%%% HISTORIQUE :  Aucun                                                                       %%%
%%%                                                                                           %%%
%%% ALGO - REFERENCES :                                                                       %%%
%%%   * https://www.figma.com/file/2jdpUKlbHxAkkBcXpv5S9a (schéma de l'algorithme)            %%%
%%%-------------------------------------------------------------------------------------------%%%
clc
close all
clear all

%% Pour tester un son, appeler la fonction parseSound(nom_du_son)
% parseSound("./audio/NotesPiano.wav");
parseSound("./audio/TailleHaie.mp3");

function parseSound(audio)
    disp("");
    disp("---------" + audio + "---------");
    disp("");
		[T, Psilence, Pbruit, signalBruit] = Probleme4(audio);
		figure
    hold on;
    plot(T, Psilence, 'g')
    plot(T, Pbruit, 'r')
    plot(T, signalBruit);
    ylabel("Puissance (dBm)")
    xlabel("Temps (s)")
    title('Bruits et silences du son ' + audio)
    hold off;
		disp("");
end