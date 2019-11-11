#include "basicRender.hpp"
#include<vector>
#include<iostream>
#include<string>
using std::cout;
using std::endl;
using std::vector;
using std::string;



struct point {
float x;
float y;
float z;
};

struct color {
float r;
float g;
float b;
};
//NOTE:: all shapes are drawn counter-clockwise, so keep that in mind
//The verticies are drawn using indexed rendering, each new primitive (triangles and squares for now) handles its own indicies
BasicRenderer::Vertex  make_vertex(point p,color c){
  glm::vec2 pos = {p.x,p.y};
  glm::vec3 col = {c.r,c.b,c.g};
  BasicRenderer::Vertex vertex = {pos,col};
  return vertex; 

}
//uint16_t get_max_index(vector<uint16_t> indicies){
//    uint16_t max = 0;
//    for(size_t i=0;i<indicies.size();i++){
//      if(indicies[i]>max) max = indicies[i];
//    }
//  return max;
//}

//adds a triangle to the vertex and index buffers provided
void add_triangle(point x1, point x2, point x3, color c,vector<BasicRenderer::Vertex>& vertexBuffer,vector<uint16_t>& indicies){
        size_t offset = vertexBuffer.size(); 
    //first, create verticies for each point, with color c
       vertexBuffer.push_back(make_vertex(x1,c));
       vertexBuffer.push_back(make_vertex(x2,c)); 
       vertexBuffer.push_back(make_vertex(x3,c)); 
       indicies.push_back(offset);
       indicies.push_back(offset+1);
       indicies.push_back(offset+2);
  }
//accepts a point in screen space,used as the bottom left of the square with origin at the center of the screen, bounds +1 to -1 
void add_square(point bottom_left,float length, color c, vector<BasicRenderer::Vertex>& vertexBuffer, vector<uint16_t>& indicies){
        size_t offset = vertexBuffer.size();//get the size off the vertex buffer, this gives us the correct starting index
        //create the other three points
       point bottom_right {bottom_left.x+length,bottom_left.y,bottom_left.z};
       point top_right {bottom_left.x+length,bottom_left.y+length,bottom_left.z};
       point top_left {bottom_left.x,bottom_left.y+length,bottom_left.z};
      //push them to the vectors
       vertexBuffer.push_back(make_vertex(bottom_left,c));
       vertexBuffer.push_back(make_vertex(bottom_right,c));
       vertexBuffer.push_back(make_vertex(top_right,c));
       vertexBuffer.push_back(make_vertex(top_left,c));
       // populate indicies in counter_clockwise fashion 
       // first triangle
       indicies.push_back(offset+0);
       indicies.push_back(offset+1);
       indicies.push_back(offset+2);
        //second triangle
       indicies.push_back(offset+2);
       indicies.push_back(offset+3);
       indicies.push_back(offset+0);
}

//draws a body centered rectangle from start to end
void add_line(point start, point end,float thickness, color c,vector<BasicRenderer::Vertex>& vertexBuffer, vector<uint16_t>& indicies){
    


}


int main(){
  vector<BasicRenderer::Vertex> vertexBuffer;
  vector<uint16_t> indicies;
  point bottom_left_red = {-0.5f,-0.5f,0.0f};
  point bottom_left_green = {-0.4f,-0.4f,0.0f};
  
  color red = {1.0f,0.0f,0.0f};
  color green = {0.0f,0.0f,1.0f};
  add_square(bottom_left_red,1.0f,red,vertexBuffer,indicies);
  add_square(bottom_left_green,0.8f,green,vertexBuffer,indicies);
  cout<<"vertex buffer has "<< vertexBuffer.size() << " verticies in it"<<endl;
  cout<<"indicies are " << endl;
  for(size_t i=0;i<indicies.size();i++){
    cout<<" " << indicies[i]; 
    }
  cout<<endl;
  BasicRenderer renderer(vertexBuffer,indicies);
  renderer.run();
}
