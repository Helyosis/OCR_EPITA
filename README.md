# SUDO.C.R - EPITA S3 Project

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/a96baca52b294b9a84ac0b90f4c5e754)](https://www.codacy.com?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Helyosis/OCR_EPITA&amp;utm_campaign=Badge_Grade)

<img src="Ressources/L.png" width="500" height="500" />

This is project is an OCR wich could recognize and resolves SUDOKU.

## Installation
```
git clone https://github.com/Helyosis/OCR_EPITA.git
cd OCR_EPITA/src
make
```
## Usage
### How to compile 
```
git clone https://github.com/Helyosis/OCR_EPITA
cd OCR_EPITA/src/ && make
cd UserInterface/ && make
```
### How to use
Commande line:
```
./main 3.14.15 help:
[ Image mode specific options ]
   -i file: Specifiy the input file (required)
   -o file: Specify the output file (default: out.bmp)
   --show: Show the image being processed, one step at a time
[ Train mode specific options ]
   -n nb: Specifiy the number of iterations to train the neural net with (default is 100 000)
   -o file: Specify the output file to save the neural network
   --batch-size s: Specify the numbers of elements in a minibatch size (default 100)
   --nb-images n: Specify the number of image to train with. (default 8228)
   --learning-rate n / --step-size n: Specify the step size (default 0.25)
[ General options ]
   -v: Increase the verbose level (default 0), can be used up to 3 times
   --mode mode: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI (default is GUI)
   -h / --help: Show this help and quit
```
Graphical Interface:
```
cd OCR_EPITA/src/UserInterface
./interface
```
Then select your sudoku grid and click on the "solve" button and your grid will be solved automaticaly.

## User Interface
We are making a User Interface, to give a better usage of our tool.

<img src="Ressources/UI.png"/>
