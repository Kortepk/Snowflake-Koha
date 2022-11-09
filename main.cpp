#include <SFML/Graphics.hpp>  //Библиотека для отрисовки
#include <iostream>
#include <vector>
#include <cmath>

#define ANTI_ALIASING 8
#define MATH_PI 3.1415926535
#define SIN_60 0.86602540378

using namespace sf;  //Подключение пространство имён sf (чтобы не писать sf::)
using namespace std;

int WIDTH = 700, HEIGHT = 500, MAIN_N = 0;
bool CLICK = false;
vector < vector <double>> VERTEX, TEMP_VERTEX; // [0] - x [1] - y

RenderWindow window(VideoMode(WIDTH, HEIGHT), "Koh - loh", Style::Default, ContextSettings(0,0,ANTI_ALIASING));


void Start_vert(){
	VERTEX.clear();
	
	VERTEX.push_back(vector<double>());
	VERTEX[VERTEX.size()-1].push_back(150);
	VERTEX[VERTEX.size()-1].push_back(150);
	
	VERTEX.push_back(vector<double>());
	VERTEX[VERTEX.size()-1].push_back(550);
	VERTEX[VERTEX.size()-1].push_back(150);
	
	VERTEX.push_back(vector<double>());
	VERTEX[VERTEX.size()-1].push_back(350); // 350
	VERTEX[VERTEX.size()-1].push_back(450); // 450
	
	VERTEX.push_back(vector<double>());
	VERTEX[VERTEX.size()-1].push_back(150);
	VERTEX[VERTEX.size()-1].push_back(150);
}

void Pr_Vert(){
	for(int i = 0; i < VERTEX.size(); i++)
		cout<<i<<") "<<VERTEX[i][0]<<" "<<VERTEX[i][1]<<endl;
	cout<<"================"<<endl<<endl;
}

void Update(int a){
	for(a; a <= MAIN_N; a++){
		double gip = WIDTH/pow(3, a), h_ka;
		h_ka = gip * sin(MATH_PI/6);
		
		TEMP_VERTEX = VERTEX; // переносим вершины
		VERTEX.clear();
		
		VERTEX.push_back(vector<double>());
		VERTEX[VERTEX.size()-1].push_back(TEMP_VERTEX[0][0]);
		VERTEX[VERTEX.size()-1].push_back(TEMP_VERTEX[0][1]);
		
		for(int i = 0; i < TEMP_VERTEX.size()-1; i++){		
			double x_3 = (TEMP_VERTEX[i+1][0] - TEMP_VERTEX[i][0])/3, y_3 = (TEMP_VERTEX[i+1][1] - TEMP_VERTEX[i][1])/3;
			double x1 = TEMP_VERTEX[i][0] + x_3, y1 = TEMP_VERTEX[i][1] + y_3;
			double x2 = TEMP_VERTEX[i+1][0] - x_3, y2 = TEMP_VERTEX[i+1][1] - y_3;
			double x3 = x1 + (x2 - x1)/2 + (y2 - y1)*SIN_60, y3 = y1 - (x2 - x1)*SIN_60 + (y2 - y1)/2;
								
			VERTEX.push_back(vector<double>()); // какие грани треуг.
			VERTEX[VERTEX.size()-1].push_back(x1);  //x1            _
			VERTEX[VERTEX.size()-1].push_back(y1);
						
			VERTEX.push_back(vector<double>()); 
			VERTEX[VERTEX.size()-1].push_back(x3);  //            _/
			VERTEX[VERTEX.size()-1].push_back(y3);
						
			VERTEX.push_back(vector<double>()); 
			VERTEX[VERTEX.size()-1].push_back(x2);  // x2           _/\
			VERTEX[VERTEX.size()-1].push_back(y2);
			VERTEX[VERTEX.size()-1].push_back(y2);

			VERTEX.push_back(vector<double>()); 
			VERTEX[VERTEX.size()-1].push_back(TEMP_VERTEX[i+1][0]);  //            _/\_
			VERTEX[VERTEX.size()-1].push_back(TEMP_VERTEX[i+1][1]);
		}
	}
}


void Draw_Line(double x1, double y1, double x2, double y2){
	Vertex line_without_thickness[] =
	{
		Vertex(Vector2f(x1, y1)),  //Координата первой вершины
		Vertex(Vector2f(x2, y2))  //Координата второй вершины
	};
		
	line_without_thickness[0].color = Color(255, 255, 255); 	
	line_without_thickness[1].color = Color(255, 255, 255);  
 
	window.draw(line_without_thickness, 2, Lines);  //Отрисовка линии
}

 
void Render(){
	double x1, y1, x2 = VERTEX[0][0], y2 = VERTEX[0][1];
	
	for(int i = 1; i < VERTEX.size(); i++){
		x1 = VERTEX[i][0]; y1 = VERTEX[i][1];
		//cout<<i<<") "<<x2<<" "<<y2<<" "<<x1<<" "<<y1<<endl;
	
		Draw_Line(x2, y2, x1, y1);
		x2 = x1; y2 = y1;
	}
	
	Font font;//шрифт 
	font.loadFromFile("arkhip.ttf");
	Text text("N=" + to_string(MAIN_N), font, 20);
	
	text.setColor(Color(255, 128, 0));
	
	text.setPosition(WIDTH - text.getGlobalBounds().width - 5, 0);
	window.draw(text);
}


int main()
{	
	window.setKeyRepeatEnabled(true); //-------------------------против спама---------------
	Start_vert();
	
	while (window.isOpen())  
	{
		Event event; 
		while (window.pollEvent(event))  
		{
			if (event.type == Event::Closed)  
				window.close();  
				
			if (event.type == Event::Resized) 
			{
	            WIDTH = event.size.width;
	            HEIGHT = event.size.height;          
	            window.setView(
	                View(
	                   Vector2f(WIDTH / 2.0, HEIGHT / 2.0), 
	                   Vector2f(WIDTH, HEIGHT)
	                )
	            );
	            Start_vert();
	            MAIN_N = 0;
	            Update(1);
	        }
			if (event.mouseButton.button == sf::Mouse::Left) {
                CLICK = not CLICK;
                if(CLICK and MAIN_N > 0){
                    MAIN_N--;
                    Start_vert();
	                Update(1);
            	}
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                CLICK = not CLICK;
                if(CLICK){
                    MAIN_N++;
                	Update(MAIN_N);
            	}
            }     
		}
		window.clear(Color(128, 128, 200, 0));   
		
		Render();	
			
		window.display();  //Отрисовка окна
	}
	return 0;
}