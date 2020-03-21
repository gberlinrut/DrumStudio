
/* DrumStudio Kit Builder [beta]
   DrumStudio.h
   Copyright (c)2004 Gregory Berlinrut  All Rights Reserved.
   
   header file: defines structs, constants, and function headers
   [created on 12/05/03] [last modified 02/21/04]

   DISCLAIMER
   ----------
	THIS SOFTWARE IS PROVIDED BY GREGORY BERLINRUT ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL GREGORY BERLINRUT BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   ACKNOWLEDGEMENTS
   ----------------
	1. GLee.h and GLee.c courtesy of Ben Woodhouse [See README or GLee.h/c]
	2. All textures except Logo.ppm are courtesy of GRsites.com.
	3. Special thanks to Dmitry Gimzelberg for technical assistance.
*/

#ifndef DRUMSTUDIO_H_
#define DRUMSTUDIO_H_

#include <GLee.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------
			LIGHTING and TEXTURING DEFINITIONS
   -------------------------------------------------- */

typedef struct MaterialStruct{
	int num;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat n;
	char *name;
} material;

typedef struct TextureStruct{
	int num;
	char *name;
	GLubyte **texture;
} texture;

// white light
static GLfloat whitelighta[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat whitelightd[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat whitelights[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat whitelightp[] = {0.0, 0.0, 10.0, 1.0};

// colors
static material STEEL = {
	0, {0.15, 0.15, 0.15, 1.0}, {0.95, 0.95, 0.95, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Steel"};
static material BRASS1 = {
  0, {0.08, 0.065, 0.0, 1.0}, {0.804, 0.647, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Brass1"};
static material BRASS2 = {
  0, {0.01, 0.077, 0.0, 1.0}, {0.965, 0.777, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Brass2"};
static material CLEAR = {
  0, {0.5, 0.5, 0.5, 1.0}, {0.5, 0.5, 0.5, 1.0}, {0.7, 0.7, 0.7, 1.0}, 75.0, "Clear"};
static material RUBBER = {
  0, {0.01, 0.01, 0.01, 1.0}, {0.2, 0.2, 0.2, 1.0}, {0.0, 0.0, 0.0, 1.0}, 128, "Rubber"};
static material GLOSS = {
  0, {0.2, 0.2, 0.2, 1.0}, {1.0, 1.0, 1.0, 1.0}, {0.3, 0.3, 0.3, 1.0}, 128.0, "Gloss"};
static material POLISH = {
  0, {0.1, 0.1, 0.1, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128.0, "Polish"};
static material WHITE = {
  7, {0.05, 0.05, 0.05, 1.0}, {0.9, 0.9, 0.9, 1.0}, {0.5, 0.5, 0.5, 1.0}, 128.0, "White"};
static material RED = {
  6, {0.2, 0.01, 0.01, 1.0}, {0.75, 0.1, 0.1, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Red"};
static material BLUE = {
  2, {0.01, 0.01, 0.2, 1.0}, {0.1, 0.1, 0.75, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Blue"};
static material GREEN = {
  3, {0.01, 0.2, 0.01, 1.0}, {0.1, 0.75, 0.1, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Green"};
static material BLACK = {
  1, {0.01, 0.01, 0.01, 1.0}, {0.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128, "Black"};
static material ORANGE = {
  4, {0.2, 0.11, 0.013, 1.0}, {0.99, 0.553, 0.067, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128.0, "Orange"};
static material PURPLE = {
  5, {0.11, 0.0, 0.011, 1.0}, {0.41175, 0.0, 0.41175, 1.0}, {1.0, 1.0, 1.0, 1.0}, 128.0, "Purple"};

static material* ACTIVECOLOR = &RED;
static material* COLOR_LIST[] = {&BLACK, &BLUE, &GREEN, &ORANGE, &PURPLE, &RED, &WHITE};

// textures [courtesy of GRsites.com]
static GLubyte *floor, *logo, *button, *BlackMarble, *Gold, *Oceana, *Royal, *Velvet, *Verdant, *WhiteMarble;

static texture NONE = {0, "None", NULL};
static texture BLACKMARBLE = {1, "Black Marble", &BlackMarble};
static texture GOLD = {2, "Gold", &Gold};
static texture OCEANA = {3, "Oceana", &Oceana};
static texture ROYAL = {4, "Royal", &Royal};
static texture VELVET = {5, "Velvet", &Velvet};
static texture VERDANT = {6, "Verdant", &Verdant};
static texture WHITEMARBLE = {7, "White Marble", &WhiteMarble};

static texture* ACTIVETEXTURE = &NONE;
static texture* TEXTURE_LIST[] = {&NONE, &BLACKMARBLE, &GOLD, &OCEANA, &ROYAL, &VELVET, &VERDANT, &WHITEMARBLE};

// texture coordinate maps
static GLfloat TEX_S_PLANE[] = {1.0, 0.0, 0.0, 0.5};
static GLfloat TEX_T_PLANE[] = {0.0, 1.0, 0.0, 0.5};

void SetMaterial(material *mat);
GLubyte *LoadTexture(char *texfile);

/* --------------------------------------------------
			DRUM STRUCTS and DEFINITIONS
   -------------------------------------------------- */
//typedef struct drum;
typedef void (*DrumFunc)();//*);
typedef void (*MoveFunc)(int);

typedef struct DrumStruct{
	// render data
	char name[30];
	GLfloat size1, size2; // standard sizes
	material *color;
	texture *tex;
	GLfloat x, y; // floor/rack coordinates
	GLfloat R1, R2; // rotations
	GLfloat E1, E2; // extentions
	GLfloat T1, T2; // tilts
	DrumFunc Draw;
	MoveFunc Move[4];
	// save file data
	int drumtype; // arbitrarily assigned by creating functions
	int cfcnindex; // creating function option index
	int colorindex; // corresponds to ACTIVECOLOR
	int texindex; // corresponds to ACTIVETEXTURE
} drum;

static drum* drumlist[20];
static int NUMDRUMS = 0;
static int ACTIVE_DRUM = -1;
static int ACTIVE_CONTROL = 0;

// creating functions
void make_drum(char *name, GLfloat s1, GLfloat s2, DrumFunc Render, MoveFunc Move, MoveFunc Extend, int dtype, int findex);
void delete_drum(int item);

void NewBassDrum(int size);
void NewFloorTom(int size);
void NewRackTom(int size);
void NewSnareDrum(int size);

void NewHiHats(int size);
void NewCrashCymbal(int size);
void NewRideCymbal(int size);
void NewChinaCymbal(int size);
void NewSplashCymbal(int size);
void NewBellCymbal(int size);

void NewCymbalStand();
void NewHiHatStand();
void NewSnareStand();
void NewRackMount();
void NewBassPedal();
void NewDoublePedal();
void NewDrumThrone();

// rendering functions
void DrawBassDrum(drum *bassdrum);
void DrawFloorTom(drum *floortom);
void DrawRackTom(drum *racktom);
void DrawSnareDrum(drum *snaredrum);

void DrawHiHats(drum *hihats);
void DrawCrashCymbalF(drum *crashcym);
void DrawRideCymbalF(drum *ridecym);
void DrawChinaCymbalF(drum *chinacym);
void DrawCrashCymbalR(drum *crashcym);
void DrawRideCymbalR(drum *ridecym);
void DrawChinaCymbalR(drum *chinacym);
void DrawSplashCymbal(drum *splashcym);
void DrawBellCymbal(drum *bellcym);

void DrawCymbalStand(drum *stand);
void DrawHiHatStand(drum* stand);
void DrawSnareStand(drum *stand);
void DrawRackMount(drum *mount);
void DrawBassPedal(drum *pedal);
void DrawDoublePedal(drum *pedal);
void DrawDrumThrone(drum *throne);

// movement functions
void MoveFloor(int key);
void MoveRack(int key);
void Rotate(int key);
void ExtendSnare(int key);
void ExtendBoom(int key);
void Tilt(int key);

/* --------------------------------------------------
			MENU DEFINITIONS and CALLBACKS
   -------------------------------------------------- */

#define defaultmenu 0
#define FILE_MENU 1
#define DRUM_MENU 2
#define CYMBAL_MENU 3
#define HARDWARE_MENU 4
#define OPTION_MENU 5
#define FLOORCYM_MENU 6
#define RACKCYM_MENU 7
#define HIHAT_MENU 8
#define BASSDRUM_MENU 9
#define FLOORTOM_MENU 10
#define RACKTOM_MENU 11
#define SNARE_MENU 12
#define CRASH_MENU 13
#define RIDE_MENU 14
#define CHINA_MENU 15
#define SPLASH_MENU 16
#define BELL_MENU 17
#define COLORS_MENU 18
#define TEXTURE_MENU 19

// Command window menu
static short int MainMenuIndex[5] = {0, 0, 0, 0, 0};
static char* CommandMenu[] = {"      FILE", "   DRUMS", " CYMBALS", "HARDWARE", " OPTIONS"};

// ----- Menu Window Hierarchy -----
static char* DefaultMenu[] = {"","Select an option","from the Main Menu."};
// [File Menu]
static char* FileMenu[] = {"FILE", "New","Load","Save","Close"};
// [Drums Menu]
static char* DrumMenu[] = {"DRUMS","Bass Drums","Floor Toms","Rack Toms","Snare Drums"};
// + submenus
static char* BassDrumMenu[] = {"BASS DRUMS","18x20 Bass Drum","18x22 Bass Drum","20x24 Bass Drum"};
static char* FloorTomMenu[] = {"FLOOR TOMS","16x16 Floor Tom","18x18 Floor Tom"};
static char* RackTomMenu[] = {"RACK TOMS","8x8 Rack Tom","8x9 Rack Tom","9x10 Rack Tom","9x11 Rack Tom","10x12 Rack Tom","11x13 Rack Tom","11x14 Rack Tom"};
static char* SnareDrumMenu[] = {"SNARE DRUMS","6x12 Snare Drum","6x13 Snare Drum","6x14 Snare Drum"};
// [Cymbals Menu]
static char* CymbalMenu[] = {"CYMBALS","Floor Cymbals","Hi Hats","Rack Cymbals"};
// + submenus
static char* FloorCymbalMenu[] = {"FLOOR CYMBALS","Crash Cymbals","Ride Cymbals","China Cymbals"};
static char* HiHatMenu[] = {"HI HATS","12\" Hi Hats","13\" Hi Hats","14\" Hi Hats"};
static char* RackCymbalMenu[] = {"RACK CYMBALS","Crash Cymbals","Ride Cymbals","China Cymbals","Splash Cymbals","Bells"};
// + + submenus
static char* CrashMenu[] = {"CRASH CYMBALS","15\" Crash Cymbal","16\" Crash Cymbal","17\" Crash Cymbal","18\" Crash Cymbal","19\" Crash Cymbal"};
static char* RideMenu[] = {"RIDE CYMBALS","18\" Ride Cymbal","20\" Ride Cymbal","22\" Ride Cymbal"};
static char* ChinaMenu[] = {"CHINA CYMBALS","14\" China Cymbal","16\" China Cymbal","18\" China Cymbal","20\" China Cymbal"};
static char* SplashMenu[] = {"SPLASH CYMBALS","6\" Splash Cymbal","8\" Splash Cymbal","10\" Splash Cymbal","12\" Splash Cymbal"};
static char* BellMenu[] = {"BELL CYMBALS","6\" Bell Cymbal","8\" Bell Cymbal","10\" Bell Cymbal"};
// [Hardware Menu]
static char* HardwareMenu[] = {"HARDWARE","Cymbal Stand","Hi Hat Stand","Snare Stand","Rack Boom Mount", "Bass Pedal","Double Bass Pedal","Drum Throne"}; 
// [Options Menu
static char* OptionsMenu[] = {"OPTIONS","Set Color","Set Pattern","View Status Window","Ghost Mode","Reset All"}; 
// + submenus
static char* ColorMenu[] = {"SET COLOR","Black", "Blue","Green","Orange","Purple","Red","White"};
static char* TextureMenu[] ={"SET PATTERN","Black Marble","Gold","Oceana","Royal","Velvet","Verdant","White Marble"};

static int CurrentMenu = defaultmenu;
static short int MenuSizes[]= {2,4,4,3,7,5,3,5,3,3,2,7,3,5,3,4,4,3,7,7};

char** getActiveMenu();

// callbacks
void file_callback(int item);
void drum_callback(int item);
void cymbal_callback(int item);
void hardware_callback(int item);
void option_callback(int item);
void floorcym_callback(int item);
void rackcym_callback(int item);
void color_callback(int item);
void texture_callback(int item);
// the rest are object creating functions

static int CYMTAG = 0; // switch for differentiating between floor cymbals and rack cymbals

typedef void (*FunctionPointer)();

FunctionPointer CALLBACKS[]={file_callback, drum_callback, cymbal_callback, hardware_callback, option_callback,
								floorcym_callback, rackcym_callback, NewHiHats,
								NewBassDrum, NewFloorTom, NewRackTom, NewSnareDrum,
								NewCrashCymbal, NewRideCymbal, NewChinaCymbal, NewSplashCymbal, NewBellCymbal,
								color_callback, texture_callback};

/* --------------------------------------------------
			DISPLAY DEFINITIONS and FUNCTIONS
   -------------------------------------------------- */

static GLuint window, command, menu, objectlist, studio;

typedef struct CameraStruct{
	GLfloat spin, lift, zoom;
	GLfloat x, y;
	int mode, mouse_x, mouse_y;
} camera;

#ifdef WIN32
typedef void (__stdcall *Renderer)();
#else
typedef void (*Renderer)() __attribute__((stdcall));
#endif

static GLclampf SKY_COLOR = 0.0;
static GLenum DRAWMODE = GLU_FILL;
Renderer RenderFcn[] = {glutSolidCube, glutWireCube, glutSolidTorus, glutWireTorus};
static int SolidState = 0;
static GLfloat RACKHEIGHT = 0.84;

static camera CAM = {0.0, 0.0, 1.0, 0.0, 0.0, 0, 0, 0};
static GLfloat SCALE = 0.02; // this is 1"
static int STATUS_WINDOW = 0;

static GLdouble ClipRack[] = {0.0, -1.0, 0.0, 0.0};
static GLdouble ClipCrashDome[] = {0.0, 0.0, 1.0, -0.0125};
static GLdouble ClipRideDome[] = {0.0, 0.0, 1.0, -0.012};
static GLdouble ClipChinaDome[] = {0.0, 0.0, 1.0, -0.01};
static GLdouble ClipChinaRim[] = {0.0, 0.0, -1.0, -0.01};

static GLdouble ClipPedal[] = {0.0, -1.0, 0.0, 0.0};
static GLdouble ClipY[] = {0.0, 1.0, 0.0, 0.0};
static GLdouble ClipZ[] = {0.0, 0.0, 1.0, 0.0};

static GLdouble Shear[] = {1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							-1.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0};

void init(void);
void drawstr2d(GLuint x, GLuint y, char* text, void* style);
void redisplay_all(void);
void init_studio(void);
void reset_studio(void);

void reshape_ortho(int w, int h);
void reshape_proj(int w, int h);

void main_display(void);
void command_display(void);
void menu_display(void);
void objectlist_display(void);
void studio_display(void);

/* --------------------------------------------------
			USER INPUT and MOTION FUNCTIONS
   -------------------------------------------------- */

void command_mouse(int button, int state, int x, int y);
void menu_mouse(int button, int state, int x, int y);
void objectlist_mouse(int button, int state, int x, int y);
void studio_mouse(int button, int state, int x, int y);
void studio_move(int x, int y);
void keyboard(unsigned char key, int x, int y);
void function_keys(int key, int x, int y);

/* --------------------------------------------------
		   FILE READING and WRITING FUNCTIONS
   -------------------------------------------------- */

void savefile();
void loadfile();

#endif