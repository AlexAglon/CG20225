// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <iostream>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
#include "Texture.h" // para  skybox

// Properties
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void Animation();
void animate(void);

// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Variables de animaciones
// Variables Auto
float movAutox = 0.0;
float movAutoz = 0.0;
float orientaAuto = 0.0;
float rotLLanta = 0.0;
float varAuto = 0.0;

float movBalonx = 0.0f;
float movBalony = 0.0f;
float movBalonz = 0.0f;
float rotBalonBasket = 0.0f;
bool animBalonBasket = false;
float guardarDistancia = 0.0f;
int rutaBalon = 0;

// posiciones
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 90.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;




// Animacion Tiro Parabólico
// v es qué tan lejos llega el objeto
float    g = 9.81f, v = 30.0f, ang = 60.0f, t = 45.0f;
double    n = 3.1416;
float    i = 0.0f;


//Animacion Robot
float rotBall = 0.0f;
float rotRobot = 0.0f;
float rotRobotCabeza = 0.0f;
float rotRobotBrazoIzq = 0.0f;
float rotRobotBrazoDer = 0.0f;
float rotRobotRueda = 0.0f;
int RobotAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float Cabeza = 0.0f;
float tail = 0.0f;
float incrotCabeza = 0.0f;
float incrotBrazoIzq = 0.0f;
float incrotBrazoDer = 0.0f;

void animate(void)
{
    //Animacion de Auto
    movAutox = 100.0f * sin(varAuto);
    movAutoz = 100.0f * cos(varAuto);
    orientaAuto += 1.0f;
    varAuto += 0.015f;
    rotLLanta += 5.0f;

    // Animación de basket
    if (animBalonBasket)
    {
        if (rutaBalon == 0) {
            rotBalonBasket -= 6.1f;
            // ANIMACION DE TIRO PARABÓLICO

            i += 0.12f;
            movBalonx = guardarDistancia + 0 + i * v * cos(ang * n / 180);
            movBalony = 0 + (i * v * sin(ang * n / 180) - (g * i * i) / 2);
            if (movBalony <= 0.0f)
            {
                if (movBalonx >= 1400.0) {
                    rutaBalon++;
                }
                i = 0.0f;
                guardarDistancia = movBalonx;



            }
        }
        if (rutaBalon == 1) {
            rotBalonBasket -= 6.1f;
            // ANIMACION DE TIRO PARABÓLICO

            i += 0.12f;
            movBalonx = guardarDistancia - 0 - i * v * cos(ang * n / 180);
            movBalony = 0 + (i * v * sin(ang * n / 180) - (g * i * i) / 2);
            if (movBalony <= 0.0f)
            {
                if (movBalonx <= 0) {
                    rutaBalon--;
                }
                i = 0.0f;
                guardarDistancia = movBalonx;



            }
        }
    }
}

//KeyFrames Robot
float RobotPosX, RobotPosY, RobotPosZ;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {

    float rotRobot;
    float rotRobotInc;
    float RobotPosX;
    float RobotPosY;
    float RobotPosZ;
    float rotRobotCabeza;
    float rotRobotBrazoIzq;
    float rotRobotBrazoDer;
    float rotRobotRueda;
    float incX;
    float incY;
    float incZ;
    float incrotCabeza;
    float incrotBrazoIzq;
    float incrotBrazoDer;



}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

    printf("frameindex %d\n", FrameIndex);

    KeyFrame[FrameIndex].RobotPosX = RobotPosX;
    KeyFrame[FrameIndex].RobotPosY = RobotPosY;
    KeyFrame[FrameIndex].RobotPosZ = RobotPosZ;
    KeyFrame[FrameIndex].rotRobotCabeza = rotRobotCabeza;
    KeyFrame[FrameIndex].rotRobotBrazoDer = rotRobotBrazoDer;
    KeyFrame[FrameIndex].rotRobotBrazoIzq = rotRobotBrazoIzq;
 

 


    KeyFrame[FrameIndex].rotRobot = rotRobot;


    FrameIndex++;
}

void resetElements(void)
{
    RobotPosX = KeyFrame[0].RobotPosX;
    RobotPosY = KeyFrame[0].RobotPosY;
    RobotPosZ = KeyFrame[0].RobotPosZ;
    rotRobotBrazoDer = KeyFrame[0].rotRobotBrazoDer;
    rotRobotBrazoIzq = KeyFrame[0].rotRobotBrazoIzq;
    rotRobotCabeza = KeyFrame[0].rotRobotCabeza;

    rotRobot = KeyFrame[0].rotRobot;

}
void interpolation(void)
{

    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].RobotPosX - KeyFrame[playIndex].RobotPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].RobotPosY - KeyFrame[playIndex].RobotPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].RobotPosZ - KeyFrame[playIndex].RobotPosZ) / i_max_steps;
    KeyFrame[playIndex].incrotBrazoDer = (KeyFrame[playIndex + 1].rotRobotBrazoDer - KeyFrame[playIndex].rotRobotBrazoDer) / i_max_steps;
    KeyFrame[playIndex].incrotBrazoIzq = (KeyFrame[playIndex + 1].rotRobotBrazoIzq - KeyFrame[playIndex].rotRobotBrazoIzq) / i_max_steps;
    KeyFrame[playIndex].incrotCabeza = (KeyFrame[playIndex + 1].rotRobotCabeza - KeyFrame[playIndex].rotRobotCabeza) / i_max_steps;

    KeyFrame[playIndex].rotRobotInc = (KeyFrame[playIndex + 1].rotRobot - KeyFrame[playIndex].rotRobot) / i_max_steps;

}


int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Carga de modelos y camara sintetica", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");
    

    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // CARGA DE MODELOS
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // RUTA DE MODELOS
  // Model EscenaCompleta((char*)"resources/EscenaCompleta/EscenaCompleta.obj");
    Model BalonBasket((char*)"resources/BalonBasket/BalonBasket.obj");

    // Animacion Carro
    Model CarroAnimacion((char*)"resources/AnimacionCarro/CarroAnimacion.obj");
    Model LLantaderDelantera((char*)"resources/AnimacionCarro/LLantaderDelantera.obj");
    Model LLantaDerTrasera((char*)"resources/AnimacionCarro/LLantaDerTrasera.obj");
    Model LLantaizqfrente((char*)"resources/AnimacionCarro/LLantaizqfrente.obj");
    Model LLantaizqTrasera((char*)"resources/AnimacionCarro/LLantaizqTrasera.obj");

    // RUTA DE MODELOS
    Model ZonaHotel((char*)"resources/ZonaHotel_r/zonaHotelR.obj");
    Model ZonaAlberca((char*)"resources/ZonaAlberca/ZonaAlbercas.obj");
    Model Estacionamiento((char*)"resources/Estacionamiento/Estacionamiento.obj");
    Model ZonaRestaurante((char*)"resources/ZonaRestaurante/ZonaRestaurante.obj");
    Model recepcion((char*)"resources/Recepcion/recepcion.obj");

    // Animacion Robot
    Model Cabeza((char*)"resources/Robotina/Cabeza.obj");
    Model BrazoDer((char*)"resources/Robotina/BrazoDer.obj");
    Model BrazoIzq((char*)"resources/Robotina/BrazoIzq.obj");
    Model Cuerpo((char*)"resources/Robotina/Cuerpo.obj");
    Model Rueda((char*)"resources/Robotina/Rueda.obj");
 
    // Variables Auto
    float movAutox = 0.0;
    float movAutoz = 0.0;
    float orientaAuto = 0.0;
    float rotLLanta = 0.0;
    float varAuto = 0.0;

   // Variables Balon
    float movBalonx = 0.0f;
    float movBalony = 0.0f;
    float movBalonz = 0.0f;
    float rotBalonBasket = 0.0f;
    bool animBalonBasket = false;
    float guardarDistancia = 0.0f;
    int rutaBalon = 0;


    // Posicion de los vertices del SkyBox

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };


    //KeyFrames
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        KeyFrame[i].RobotPosX = 0;
        KeyFrame[i].RobotPosY = 0;
        KeyFrame[i].RobotPosZ = 0;
        KeyFrame[i].incX = 0;
        KeyFrame[i].incY = 0;
        KeyFrame[i].incZ = 0;
        KeyFrame[i].rotRobot = 0;
        KeyFrame[i].rotRobotCabeza = 0;
        KeyFrame[i].rotRobotBrazoIzq = 0;
        KeyFrame[i].rotRobotBrazoDer = 0;
        KeyFrame[i].rotRobotRueda = 0;
        KeyFrame[i].rotRobotInc = 0;
    }


    // Indices de la caja del skybox
    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,2,3,
        4,5,6,7,
        8,9,10,11,
        12,13,14,15,
        16,17,18,19,
        20,21,22,23,
        24,25,26,27,
        28,29,30,31,
        32,33,34,35
    };

    /// Carga del Buffer
    GLuint EBO, VBO, VAO;
    glGenBuffers(1, &EBO);
   // glGenBuffers(1, &VBO);
   // glGenBuffers(1, &VAO);
    
   // glBindVertexArray(VAO);
   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Skybox  Configuracion

    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  
    // Carga de texturas Skybox

    vector<const GLchar*>faces;
    faces.push_back("SkyBox/right.jpg");
    faces.push_back("SkyBox/left.jpg");
    faces.push_back("SkyBox/top.jpg");;
    faces.push_back("SkyBox/bottom.jpg");
    faces.push_back("SkyBox/front.jpg");
    faces.push_back("SkyBox/back.jpg");

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        Animation(); // llamada a funcion animacion kframes
        animate(); //llamada a funcion animacion carro


        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Dibujar modelos 
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
       
 
                // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc = glGetUniformLocation(shader.Program, "projection");
        // Dibujo Animacion KFrames Robotina

// Dibujar modelos 
        ZonaHotel.Draw(shader);
        //Zona Albercas 
        ZonaAlberca.Draw(shader);
        //Zona Albercas 
        Estacionamiento.Draw(shader);
        //Zona Restaurante 
        ZonaRestaurante.Draw(shader);
        //Recepcion 
        recepcion.Draw(shader);
       
       
        // Balón de Baloncesto Animacion
    //    glm::mat4 ballModel = glm::mat4(1.0f); // Matriz inicial de identidad
     //   ballModel = glm::translate(ballModel, glm::vec3(movBalonx - 741.048f, movBalony + 12.493f, movBalonz - 681.41f)); // Traslación
     //   ballModel = glm::rotate(ballModel, glm::radians(rotBalonBasket), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación
     //   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ballModel)); // Asignación de la matriz al shader
     //   BalonBasket.Draw(shader); // Renderizado del modelo


// Carro Animación
        glm::mat4 carModel = glm::mat4(1);
        carModel = glm::translate(carModel, glm::vec3(movAutox + 8.858f, +1.926f, movAutoz + 17.198f));
        carModel = glm::rotate(carModel, glm::radians(orientaAuto), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(carModel));
        CarroAnimacion.Draw(shader);

        // Llanta Derecha Frontal Si
        glm::mat4 wheelModel =  glm::mat4(1);
        wheelModel = glm::translate(wheelModel, glm::vec3(8.315f, 1.603, +18.162));
        wheelModel = glm::rotate(wheelModel, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wheelModel = glm::rotate(wheelModel, glm::radians(rotLLanta), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wheelModel));
        LLantaderDelantera.Draw(shader);

        // Llanta Izquierda Frontal // Si
        wheelModel = glm::mat4(1); // Reiniciamos desde la posición base del carro
        wheelModel = glm::translate(wheelModel, glm::vec3(+9.398f, 1.599f, +18.149f));
        wheelModel = glm::rotate(wheelModel, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wheelModel = glm::rotate(wheelModel, glm::radians(rotLLanta), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wheelModel));
        LLantaizqfrente.Draw(shader);

        // Llanta Derecha Trasera Si
        wheelModel = glm::mat4(1); // Reiniciamos desde la posición base del carro
        wheelModel = glm::translate(wheelModel, glm::vec3(8.314, 1.619, +16.406));
        wheelModel = glm::rotate(wheelModel, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wheelModel = glm::rotate(wheelModel, glm::radians(rotLLanta), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wheelModel));
        LLantaDerTrasera.Draw(shader);

        // Llanta Izquierda Trasera
        wheelModel = glm::mat4(1); // Reiniciamos desde la posición base del carro
        wheelModel = glm::translate(wheelModel, glm::vec3(9.402f, 1.609f, +16.388f));
        wheelModel = glm::rotate(wheelModel, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wheelModel = glm::rotate(wheelModel, glm::radians(rotLLanta), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(wheelModel));
        LLantaizqTrasera.Draw(shader);


                
       //Cuerpo Listo
        model = glm::translate(model, glm::vec3(0.006f, 2.313f, 0.053f));
        model = glm::translate(model, glm::vec3(RobotPosX, RobotPosY, RobotPosZ));
        model = glm::rotate(model, glm::radians(rotRobot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Cuerpo.Draw(shader);

        //Cabeza Listo
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.014f, 2.645f, 0.052));
        model = glm::translate(model, glm::vec3(RobotPosX, RobotPosY, RobotPosZ));
        model = glm::rotate(model, glm::radians(rotRobotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Cabeza.Draw(shader);

        //Brazo Derecho
      //  model = modelTemp;
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.171f, 2.456f, 0.1f));
        model = glm::translate(model, glm::vec3(RobotPosX, RobotPosY, RobotPosZ));
        model = glm::rotate(model, glm::radians(rotRobotBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        BrazoDer.Draw(shader);

        //brazo izquierdo
      //  model = modelTemp;
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.181f, 2.42f, 0.0));
        model = glm::translate(model, glm::vec3(RobotPosX, RobotPosY, RobotPosZ));
        model = glm::rotate(model, glm::radians(rotRobotBrazoIzq), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        BrazoIzq.Draw(shader);

        //Rueda Listo
      //  model = modelTemp;
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.049f,1.871f, 0.056f));
        model = glm::translate(model, glm::vec3(RobotPosX, RobotPosY, RobotPosZ));
        model = glm::rotate(model, glm::radians(rotRobotRueda), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Rueda.Draw(shader);
        


        // Dibujo del skybox
        glDepthFunc(GL_LEQUAL);
        skyboxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);


        // Swap the buffers
        glfwSwapBuffers( window );

    }

   // glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &skyboxVBO);
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
    //Controles del robot

    if (keys[GLFW_KEY_2])
    {

        rotRobotCabeza += 0.1f;

    }

    if (keys[GLFW_KEY_3])
    {

        rotRobotCabeza -= 0.1f;

    }

    if (keys[GLFW_KEY_H])
    {
        rotRobotBrazoDer += 0.1;
    }

    if (keys[GLFW_KEY_Y])
    {
        rotRobotBrazoDer -= 0.1;
    }

    if (keys[GLFW_KEY_G])
    {
        rotRobotBrazoIzq -= 0.1;
    }

    if (keys[GLFW_KEY_J])
    {
        rotRobotBrazoIzq += 0.1;
    }
    if (keys[GLFW_KEY_T])
    {
        animBalonBasket ^= true;
    }
    if (keys[GLFW_KEY_Q])
    {
        i = 0.0;
        guardarDistancia = 0.0f;
        rotBalonBasket = 0.0f;
        movBalonx = 0.0f;
        movBalony = 0.0f;
        animBalonBasket = false;
   
}
    }

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
    if (keys[GLFW_KEY_L])
    {
        if (play == false && (FrameIndex > 1))
        {

            resetElements();
            //First Interpolation				
            interpolation();

            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        }
        else
        {
            play = false;
        }

    }

    if (keys[GLFW_KEY_K])
    {
        if (FrameIndex < MAX_FRAMES)
        {
            saveFrame();
        }

    }

 

 
}

// CREACION DEL TXT para Kframes
void checkFileExistence(void) {
    ifstream inputFile("animation.txt");
    vector<float> data;  // Array to store data from the file
    int index = 0;

    // Check if the file exists
    if (!inputFile.is_open()) {
        // If the file doesn't exist, create it and populate it with data
        cout << "The file does not exist. Creating...\n";
        ofstream outputFile("animation.txt");
        outputFile.close();
    }
    else {
        // If the file exists, read the data and store it in the vector
        cout << "The file already exists. Reading data...\n";
        float number;
        while (inputFile >> number) {
            data.push_back(number);
        }
        inputFile.close();  // Close the file after reading

        // Display the data read
        cout << "Data read successfully\n";
        for (int i = 0; i < data.size() / 12; i++) {
            KeyFrame[i].RobotPosX = data[index++];
            KeyFrame[i].RobotPosY = data[index++];
            KeyFrame[i].RobotPosZ = data[index++];
            KeyFrame[i].rotRobot = data[index++];
            KeyFrame[i].rotRobotCabeza = data[index++];
            KeyFrame[i].rotRobotBrazoDer= data[index++];
            KeyFrame[i].rotRobotBrazoIzq = data[index++];
            KeyFrame[i].rotRobotRueda = data[index++];
           // KeyFrame[i].rotRightFoot = data[index++];
           // KeyFrame[i].rotLeftFoot = data[index++];

        }

        FrameIndex = (data.size() / 12);
    }
}


void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}



// Animacion del Robot
void Animation() {

    if (play)
    {
        if (i_curr_steps >= i_max_steps) //end of animation between frames?
        {
            playIndex++;
            if (playIndex > FrameIndex - 2)	//end of total animation?
            {
                printf("termina anim\n");
                playIndex = 0;
                play = false;
            }
            else //Next frame interpolations
            {
                i_curr_steps = 0; //Reset counter
                //Interpolation
                interpolation();
            }
        }
        else
        {
            //Draw animation
            RobotPosX += KeyFrame[playIndex].incX;
            RobotPosY += KeyFrame[playIndex].incY;
            RobotPosZ += KeyFrame[playIndex].incZ;


           
            rotRobotCabeza += KeyFrame[playIndex].incrotCabeza;
            rotRobotBrazoIzq += KeyFrame[playIndex].incrotBrazoIzq;
            rotRobotBrazoDer += KeyFrame[playIndex].incrotBrazoDer;

            rotRobot += KeyFrame[playIndex].rotRobotInc;

            i_curr_steps++;
        }

    }

}