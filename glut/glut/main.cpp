#define x first
#define y second
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <GLUT/glut.h>
#include <time.h>
#include <cmath>
#include <set>
#include <string>


using namespace std;

float randomFloat(){
    
    return (float)(rand()) / (float)(RAND_MAX);
}


class GeomtryObject{
protected:
    float x_center;
    float y_center;
    float x_speed;
    float y_speed;
    float side;
public:
    friend void TimerFunction(int value);
    pair<float,float> Get_x_y(){
        return {x_center,y_center};
    }
    float Get_side(){
        return side;
    }
    void Set_x_y(int x_center, int y_center){
        this->x_center = x_center;
        this->y_center = y_center;
    }
    
    GeomtryObject(){};
    
    virtual void Draw() = 0;
    virtual int crash_action(GeomtryObject* crashed_in) = 0;
    virtual string Get_type() = 0;
    virtual ~GeomtryObject() =0;

};

GeomtryObject:: ~GeomtryObject(){};


class Circle : public GeomtryObject{

public:

    Circle(float x_center, float y_center, float radius, float x_speed, float y_speed){

        this->x_center = x_center;
        this->y_center = y_center;
        this->side = radius;
        this->x_speed = x_speed;
        this->y_speed = y_speed;
    }

    void Draw() override {
        
        int N=265;
        glColor3f(0.0f, 0.0f,1.0f);
        glBegin(GL_POLYGON);

        for (int i = 0; i <= N; i++) {

            glVertex2f(x_center + side*cos(2*3.14/N*i),y_center + side*sin(2*3.14/N*i));
            
        }

        glEnd();
        
    }
    
    int crash_action(GeomtryObject* crashed_in) override{

        if (crashed_in->Get_type() == "Square"){
            
                        
            if (side < crashed_in->Get_side()){
                return 1;
            } else{
                side += crashed_in->Get_side();
                return 0;
            }
        }

        if (crashed_in->Get_type() == "Triangle"){
            
            if (side < crashed_in->Get_side()){
                return 1;
            } else {
                
                side -= crashed_in->Get_side();
                return 0;
            }
            
            
            
        }
        return 0;
    }
    
    string Get_type () override{
        return "Circle";
    }
    
    ~Circle(){};
};


class Square : public GeomtryObject{
public:
    
    Square(float x_center, float y_center, float side, float x_speed, float y_speed){
        
        this->x_center = x_center;
        this->y_center = y_center;
        this->side = side;
        this->x_speed = x_speed;
        this->y_speed = y_speed;
        
        
    }
    
    void Draw() override {
        
        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_QUADS);
        glVertex2d(x_center - side, y_center - side);
        glVertex2d(x_center - side, y_center + side);
        glVertex2d(x_center + side, y_center + side);
        glVertex2d(x_center + side, y_center - side);
        glEnd();
        
    }
    
    int crash_action(GeomtryObject* crashed_in) override{

//        if (strcmp(typeid(*crashed_in).name(), "6Circle") == 0){
//            if (side < crashed_in->Get_side()){
//                return 1;
//            } else {
//
//                side -= crashed_in->Get_side();
//                return 0;
//            }
//        }
//
//        if (strcmp(typeid(*crashed_in).name() , "8Triangle") == 0){
//            if (side < crashed_in->Get_side()){
//                return 1;
//            } else{
//                side += crashed_in->Get_side();
//                return 0;
//            }
//        }
        
        
        if ( crashed_in->Get_type() ==  "Circle"){
            if (side < crashed_in->Get_side()){
                return 1;
            } else {
                
                side -= crashed_in->Get_side();
                return 0;
            }
        }

        if (crashed_in->Get_type() == "Triangle"){
            if (side < crashed_in->Get_side()){
                return 1;
            } else{
                side += crashed_in->Get_side();
                return 0;
            }
        }
        return 0;
        
    }
    
    string Get_type () override{
        return "Square";
    }
    ~Square(){};
    
};

class Triangle : public GeomtryObject{
private:
public:
    
    Triangle(float x_center, float y_center, float side, float x_speed, float y_speed){
        
        this->x_center = x_center;
        this->y_center = y_center;
        this->side = side;
        this->x_speed = x_speed;
        this->y_speed = y_speed;
    }
    
    void Draw() override {
        
        glColor3f(0.0f, 1.0f,0.0f);
        glBegin(GL_TRIANGLES);//рисуем зеленый треугольник
        glVertex2d(x_center - side, y_center - 1.73*side/3);
        glVertex2f(x_center, y_center + 1.73*side*2/3);
        glVertex2d(x_center + side, y_center - 1.73*side/3);
        glEnd();
        
    }
    
    int crash_action(GeomtryObject* crashed_in) override{
        
        if (crashed_in->Get_type() == "Square"){
            if (side < crashed_in->Get_side()){
                return 1;
            } else {

                side -= crashed_in->Get_side();
                return 0;
            }
            
        }

        if (crashed_in->Get_type() == "Circle"){
            
            if (side < crashed_in->Get_side()){
                return 1;
            } else{
                side += crashed_in->Get_side();
                return 0;
            }
            
        }
        return 0;
    }
    
    string Get_type () override{
        return "Triangle";
    }
    
    ~Triangle(){};
};


GLfloat windowWidth = 200;
GLfloat windowHeight = 200;

vector<GeomtryObject*> objects;

void RenderScene(void){
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i<objects.size();i++){
        objects[i]->Draw();
    }

    glutSwapBuffers();
}

//#define x1 objects[i]->x_center
//#define y1 objects[i]->y_center

void TimerFunction(int value){
    
    set<GeomtryObject*> to_delete;
    
    for (auto i = objects.begin(); i != objects.end();i++){
        for (auto j = i+1; j != objects.end();j++){
            if (sqrt(pow((*i)->x_center - (*j)->x_center,2) + pow((*i)->y_center - (*j)->y_center,2)) < (*i)->side + (*j)->side){
               
                bool status_1 = (*i)->crash_action(*j);
                bool status_2 = (*j)->crash_action(*i);
                
                if (status_1){
                    
                    GeomtryObject* temp = *i;
                    i = objects.erase(i) - 1;
                    delete temp;
                    break;
                    
                }
                if (status_2){
                    int n = j - i - 1;
                    GeomtryObject* temp = *j;
                    j = objects.erase(j) - 1;
                    i = j - n;
                    delete temp;
                }
            }
        }
        
    }
    
    for (int i = 0; i<objects.size();i++){
        
        if (objects[i]->x_center > windowWidth - objects[i]->side) {
            
            objects[i]->x_speed = - objects[i]->x_speed;
            objects[i]->x_center = windowWidth - objects[i]->side ;
            
            
        } else if(objects[i]->x_center < -windowWidth + objects[i]->side){
            objects[i]->x_speed = - objects[i]->x_speed;
            objects[i]->x_center = -windowWidth + objects[i]->side;
        }
        
        if (objects[i]->y_center > windowHeight - objects[i]->side){
            objects[i]->y_speed = - objects[i]->y_speed;
            objects[i]->y_center = windowHeight - objects[i]->side;
        }
        else if(objects[i]->y_center < -windowHeight + objects[i]->side){
            objects[i]->y_speed = - objects[i]->y_speed;
            objects[i]->y_center = -windowHeight + objects[i]->side;
        }
        objects[i]->x_center +=  objects[i]->x_speed;
        objects[i]->y_center += objects[i]->y_speed;

    }
    
    glutPostRedisplay();
    glutTimerFunc(33,TimerFunction,1);
}



int main(int argc, char **argv){

    
    srand((int)time(0));
    
    for (int i = 0;i<22;i++){
        float x_c = (-2*randomFloat() + 1)*200;
        float y_c = (-2*randomFloat() + 1)*200;
        float x_speed = randomFloat()*5;
        float y_speed = randomFloat()*5;
        float side = 2;
        Square* new_object = new Square(x_c,y_c,side,x_speed,y_speed);
        objects.push_back(new_object);
    }
    for (int i = 0;i<15;i++){
        float x_c = (-2*randomFloat() + 1)*200;
        float y_c = (-2*randomFloat() + 1)*200;
        float x_speed = randomFloat()*5;
        float y_speed = randomFloat()*5;
        float side = 2;
        Triangle* new_object = new Triangle(x_c,y_c,side,x_speed,y_speed);
        objects.push_back(new_object);
    }
    
    for (int i = 0;i<15;i++){
        float x_c = (-2*randomFloat() + 1)*200;
        float y_c = (-2*randomFloat() + 1)*200;
        float x_speed = randomFloat()*5;
        float y_speed = randomFloat()*5;
        float side = 2;
        Circle* new_object = new Circle(x_c,y_c,side,x_speed,y_speed);
        objects.push_back(new_object);
    }
    
    
    
    
    glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutInitWindowPosition(200, 200);
   
   glutCreateWindow("Window");
   glClearColor(0, 0, 0, 0);

   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-200, 200, -200, 200, -200, 200);
   glutDisplayFunc(RenderScene);
    glutTimerFunc(33,TimerFunction,1);
    glutMainLoop();
    return 0;
}


