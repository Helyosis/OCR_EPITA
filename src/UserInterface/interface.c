#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include <ctype.h>
//#include "SDL/SDL.h"
#include <stdbool.h>
#include <err.h>
#include "../ImageProcessing/GrayScale.h"
#include "../ImageProcessing/NoiseReduction.h"
#include "../ImageProcessing/BlackAndWhite.h"
#include "../ImageProcessing/HoughTransform.h"
#include "./utilsUI.h"

// GtK Items
GtkWidget	*main_window;
GtkWidget	*fixed1;
GtkWidget	*button_exploit;
GtkWidget	*img_chooser;
GtkWidget	*sudoku_img;
GtkWidget	*menu1;
GtkWidget	*menu_new;
GtkWidget	*menu_open;
GtkWidget	*menu_save;
GtkWidget	*menu_save_as;
GtkWidget	*menu_quit;
GtkWidget	*greyscale;
GtkWidget	*gaussian;
GtkWidget	*thresholding;
GtkWidget	*kuwahara;
//GtkWidget	*rotation;
GtkBuilder	*builder;
GtkWidget	*hough;

//Toggle events
//int GREYSCALE = 0;
//int GAUSSIAN = 0;
//int THRESHOLDING = 0;
//int KUWAHARA = 0;
int initInterface(int argc,char *argv[]){
	gtk_init(&argc, &argv); // init GtK
	
	// SETUP XML.glade -> GtK window
	builder = gtk_builder_new_from_file("main.glade");
	main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	
	g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder,NULL);
	
	// Init GTK objects
	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1")); 
	button_exploit = GTK_WIDGET(gtk_builder_get_object(builder, "button_exploit")); 
	img_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "img_chooser"));
	menu1 = GTK_WIDGET(gtk_builder_get_object(builder, "menu1")); 
	menu_new = GTK_WIDGET(gtk_builder_get_object(builder, "menu_new"));
	menu_open = GTK_WIDGET(gtk_builder_get_object(builder, "menu_open"));
	menu_save = GTK_WIDGET(gtk_builder_get_object(builder,"menu_save"));
	menu_save_as = GTK_WIDGET(gtk_builder_get_object(builder, "menu_save_as"));
	menu_quit = GTK_WIDGET(gtk_builder_get_object(builder, "menu_quit"));
        greyscale = GTK_WIDGET(gtk_builder_get_object(builder, "greyscale"));
        gaussian = GTK_WIDGET(gtk_builder_get_object(builder, "gaussian"));
	thresholding = GTK_WIDGET(gtk_builder_get_object(builder,"Thresholding"));
	kuwahara = GTK_WIDGET(gtk_builder_get_object(builder, "Kuwahara"));
	hough = GTK_WIDGET(gtk_builder_get_object(builder, "Hough"));
	//rotation = GTK_WIDGET(gtk_builder_get_object(builder, "Rotation"));	
	gtk_widget_show(main_window);
	
	//Init Sudoku_img
	sudoku_img = NULL; 

	gtk_main();
	return EXIT_SUCCESS;
}
int main(int argc, char **argv){
	initInterface(argc,argv);
}
//reload img
void reload_img(char *path){
	int vertical = 60;
        int horizontal = 20;
	if(sudoku_img){
        	gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img); //If img already exist remove it
        	printf("[-] deleting older input\n");
        }

	sudoku_img = gtk_image_new_from_file(path);
        gtk_container_add(GTK_CONTAINER(fixed1), sudoku_img);
        gtk_widget_show(sudoku_img);
        gtk_fixed_move (GTK_FIXED(fixed1), sudoku_img, horizontal, vertical); //set hte img at the right place
}

void on_button_close(){
	
	// check if the img is already loaded
	if(sudoku_img){
                gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img); //If img already exist remove it
                printf("[-] deleting older input\n");
        }
	printf("[E] They isn't img yet\n");

}

void on_img_chooser_file_set(GtkFileChooserButton *f){
	
	int vertical = 60;
	int horizontal = 20;
	//Debug
	printf("[+] Working with this file as input : %s\n", gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(f)));
	if(sudoku_img){
		gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img); //If img already exist remove it and put the new one
		printf("[-] deleting older input\n");
	}
	sudoku_img = gtk_image_new_from_file(gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(f)));
	gtk_container_add(GTK_CONTAINER(fixed1), sudoku_img);
	gtk_widget_show(sudoku_img);
	gtk_fixed_move (GTK_FIXED(fixed1), sudoku_img, horizontal, vertical); //set hte img at the right place
}


void on_new_activated(){
	on_button_close();
}

void on_open_activated(){
	GtkWidget *dialog;
	dialog = gtk_file_chooser_dialog_new("Choose a file",GTK_WINDOW(main_window),GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);
	gtk_widget_show_all(dialog);
	gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
	//If choosed a file
	if(resp==GTK_RESPONSE_OK){
		int vertical = 60;
        	int horizontal = 20;
		printf("[+] Working with this file as input : %s\n",gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
		if(sudoku_img){
			gtk_container_remove(GTK_CONTAINER(fixed1),sudoku_img);
			printf("[-] deleting older input\n");
		}
		sudoku_img = gtk_image_new_from_file(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
		gtk_container_add(GTK_CONTAINER(fixed1), sudoku_img);
        	gtk_widget_show(sudoku_img);
        	gtk_fixed_move(GTK_FIXED(fixed1), sudoku_img, horizontal, vertical); //set the img at the right place
		gtk_widget_destroy(dialog);
		
		//SDL_Surface *image = load_image(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
		//save_image(image,"Image/gaussian.png");
	}
	//If choosed cancel
	else{
		printf("[-] deleting dialogbox\n");
		gtk_widget_destroy(dialog);
	}
}

void on_quit_activated(){
	gtk_main_quit();
}
//Auto exploit
void on_button_exploit_activate(){

}

//greyscale
void on_greyscale_toggled(){
	printf("[+] Greyscale\n");
	SDL_Surface *original_image = IMG_Load("Image/actual.png");
	SDL_Surface *image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
	
	//apply filter
	Apply_grayscale_filter(image);
	
	//Saves tmp + actual_img
	save_image(image,"Image/greyscale.png");
	
	reload_img("Image/greyscale.png");
}
void on_gaussian_toggled(){
	printf("[+] GaussianBlur\n");
        
	SDL_Surface *original_image = IMG_Load("Image/actual.png");
	SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
        
	//apply filter
	GaussianBlur_inPlace(image);
        
	//Saves tmp + actual_img
	save_image(image,"Image/gaussian.png");
        reload_img("Image/gaussian.png");
}
//Thresholding
void on_th_toggled(){
	printf("[+] Thresholding\n");

        SDL_Surface *original_image = IMG_Load("Image/actual.png");
        SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);
        
	//apply filter
	AdaptiveThresholding_inPlace(image);
        
	//Saves tmp + set actual_img
	save_image(image,"Image/thresholding.png");
        reload_img("Image/thresholding.png");
}

void on_Kuwahara_toggled(){
        printf("[+] Kuwahara\n");

        SDL_Surface *original_image = IMG_Load("Image/actual.png");
        SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
        original_image, SDL_PIXELFORMAT_ARGB8888, 0);

        //apply filter
        KuwaharaFilter_inPlace(image);

        //Saves tmp + set actual_img
        save_image(image,"Image/Kuwahara.png");
        reload_img("Image/Kuwahara.png");
}
void on_hough_toggled(){
    printf("[+] HoughTransform\n");
	if(1){
        printf("BBBBBB");
		SDL_Surface *original_image = IMG_Load("Image/thresholding.png");
	    SDL_Surface *image = image = SDL_ConvertSurfaceFormat(
    	original_image, SDL_PIXELFORMAT_ARGB8888, 0);
    	//apply filter
        DrawHoughlines(image,HoughTransform(image));
		printf("AAAAA");
        //Saves tmp + set actual_img
        save_image(image,"Image/Hough.png");
        reload_img("Image/Hough.png");
		//fclose(file);
	}
	else
		errx(1,"You need to apply thresholding first");

}

