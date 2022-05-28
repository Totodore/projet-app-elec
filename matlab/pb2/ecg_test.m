clear all;

[f1, f2] = ecg("./audio/100.wav", true);

fprintf("Fréquence par détection des crêtes %f\n", f1);
fprintf("Fréquence par détection du pic maximum de la FFT %f\n", f2);