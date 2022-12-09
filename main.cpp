#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include "imgui/imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"


typedef int node_t;

class NodeWithCost{
	public:
		node_t node;
		int cost;
		NodeWithCost(node_t node, int cost){
			this->node=node;
			this->cost=cost;
		}

		bool operator ==(const NodeWithCost& nd2){
			return this->node==nd2.node;
		}
		bool operator >=(const NodeWithCost& nd2){
			return this->node>=nd2.node;
		}
		bool operator <=(const NodeWithCost& nd2){
			return this->node>=nd2.node;
		}
		int operator -(const NodeWithCost& nd2){
			return this->node-nd2.node;
		}
		int operator +(const NodeWithCost& nd2){
			return this->node+nd2.node;
		}
		int operator *(const NodeWithCost& nd2){
			return this->node*nd2.node;
		}
};

class Graph{
	public:
		std::vector<std::vector<NodeWithCost>> graph;

		Graph(std::vector<std::vector<NodeWithCost>> initialVector){
			this->graph = initialVector;
		}

		Graph(){

		}

		//	return the node indexes of the link that has the samllest cost
		int findInitialLink(std::vector<node_t>& t, int& cost){
			int minimal=21474836;
			node_t node1,node2;
			for (int i = 0; i < this->graph.size(); i++)
			{
				for (int j = 0; j < this->graph[i].size(); j++)
				{
					if (this->graph[i][j].cost<minimal)
					{
						minimal = 	this->graph[i][j].cost;
						node1 = i;
						node2 = this->graph[i][j].node;
					}
				}
			}

			cost=minimal;
			
			std::cout<<"the cost of "<< (char)('A' + node1) <<" " << (char)('A' + node2)<<" is "<<minimal<<'\n';
			t.push_back(node1);
			t.push_back(node2);
			return cost;
		}

		NodeWithCost getNextNodeIndex(std::vector<bool>& visitedNodes,int currentNode){
			int minimalCost = 21474836;
			int next=-1;
			for (int i = 0; i < this->graph[currentNode].size(); i++)
			{
				node_t nextNeighbour = this->graph[currentNode][i].node;
				int nextNeighbourCost = this->graph[currentNode][i].cost;
				if(!visitedNodes[nextNeighbour]){
					if (minimalCost>nextNeighbourCost)
					{
						minimalCost = nextNeighbourCost;
						next = nextNeighbour;
					}
					
				}
			}

			return NodeWithCost(next,minimalCost);
			
		}

		void exploreSubTree(std::vector<bool>& exploredNodes,std::vector<node_t>& tmpPath,std::vector<node_t>& permaPath,int& cost,int& currentCost,int currentNode){
			int min=INT16_MAX;
			bool done=true;
			for (int j = 0; j<this->graph[currentNode].size(); j++)
			{
				int nexNodeToExplore = this->graph[currentNode][j].node;
				int nexNodeToExploreCost = this->graph[currentNode][j].cost;

				if(exploredNodes[nexNodeToExplore])
					continue;

				done=false;

				exploredNodes[nexNodeToExplore]=true;
				currentCost+=nexNodeToExploreCost;
				tmpPath.push_back(nexNodeToExplore);
				exploreSubTree(exploredNodes,tmpPath,permaPath,cost,currentCost,nexNodeToExplore);
				tmpPath.pop_back();
				currentCost+=nexNodeToExploreCost;
				exploredNodes[nexNodeToExplore]=false;
			}

			if (done)
			{
				if(currentCost<cost){
					cost=currentCost;
					permaPath = tmpPath;
				}
			}
		}

		int findTSpath_direct(std::vector<node_t>& resultingNodes){
		
			// choose random point 
			// so we choose the first one
			std::vector<bool> explored(this->graph.size(),false);
			std::vector<node_t> tmpPath,permaPath;
			explored[0]=true;

			int cost,currentCost;
			exploreSubTree(explored,tmpPath,permaPath,cost,currentCost,0);
			resultingNodes = permaPath;

			return cost;
			
		}

		int findTSpath(std::vector<node_t>& resultingNodes){
			int totalCost=0;

			if(this->graph.size()<3)
				exit(0);
			
			std::vector<node_t> initialNodes;
			totalCost= findInitialLink(initialNodes,totalCost);

			std::vector<bool> visited(this->graph.size(),false);
			std::vector<node_t> leftNodes;
			std::vector<node_t> rightNodes;
			rightNodes.push_back(initialNodes[0]);
			leftNodes.push_back(initialNodes[1]);

			visited[initialNodes[0]]=true;
			visited[initialNodes[1]]=true;

			int nb_visitedNodes=2;

			for (int i = 0; i < this->graph.size()-2; i++)
			{
				if (nb_visitedNodes==this->graph.size())
				{
					break;
				}
				
				// get minimal link for right side
				NodeWithCost next1 = getNextNodeIndex(visited,rightNodes.back());
				// get minimal link for left side
				NodeWithCost next2 = getNextNodeIndex(visited,leftNodes.back());

				if(next1.cost<next2.cost){
					totalCost+=next1.cost;
					visited[next1.node]=true;
					rightNodes.push_back(next1.node);
					std::cout<<"the cost of "<< (char)('A' +next1.node)<<" is "<<next1.cost<<'\n';
				}else{
					totalCost+=next2.cost;
					visited[next2.node]=true;
					leftNodes.push_back(next2.node);
					std::cout<<"the cost of "<< (char)('A' +next2.node)<<" is "<<next2.cost<<'\n';
				}
				nb_visitedNodes++;
				// std::cout<<"========================================================\n";
				// for (int l = 0; l < this->graph.size(); l++)
				// {
				// 	std::cout<<"is this node "<<l<<" visited ?"<<(visited[l]?" yes\n":" no\n");
				// }
				
			}

			//	merge paths
			std::reverse(leftNodes.begin(),leftNodes.end());

			for( int val : rightNodes){
				leftNodes.push_back(val);
			}	

			// leftNodes.insert(leftNodes.end(),rightNodes.begin(),rightNodes.end());

			//	left nodes now contains the path
			for (int k = 0; k < leftNodes.size(); k++)
			{
				std::cout<<"node : " << (char)('A' +  leftNodes[k])<<'\n';
			}
			resultingNodes =std::vector<int>();
			for (int i = 0; i < leftNodes.size(); i++)
			{
				resultingNodes.push_back(leftNodes[i]);
			}
			
			int capture = resultingNodes.back();

			//	add the last link's cost
			int lastNodeCost = std::find_if(
				this->graph[resultingNodes[0]].begin(),
				this->graph[resultingNodes[0]].end(),
			 	[capture](NodeWithCost node)->bool{
					return (node.node == capture);
				}
			)->cost;

			std::cout<< "last Cost >>>>> "<<lastNodeCost<<'\n';

			return totalCost + lastNodeCost;
			
		}

		bool addLink(node_t initialNode, node_t targetNode, int cost){
			std::size_t nbNodes =this->graph.size(); 
			if (initialNode>=nbNodes  || targetNode>=nbNodes)
				return false;

			// check if link doesn't exist already
			
			NodeWithCost nd(targetNode,cost);
			bool exist1 = std::find(this->graph[initialNode].begin(),this->graph[initialNode].end(),nd) != this->graph[initialNode].end();

			NodeWithCost nd2(initialNode,cost);
			bool exist2 = std::find(this->graph[targetNode].begin(),this->graph[targetNode].end(),nd2) != this->graph[targetNode].end();

			if(exist1 || exist2)
				return false;

			this->graph[initialNode].push_back(NodeWithCost{targetNode,cost});
			this->graph[targetNode].push_back(NodeWithCost{initialNode,cost});

			return true;
		}

		int addNode(){
			long unsigned int currentSize = this->graph.size();
			this->graph.push_back(std::vector<NodeWithCost>());
			return currentSize;
		}

		void generateComplet(int numberOfNodes){
			srand(time(nullptr));
			if(numberOfNodes==0)
				return;
			for (int i = 0; i < numberOfNodes; i++)
			{
				int currentIndex = this->addNode();
				for (int j = 0; j < this->graph.size(); j++)
				{
					if(j!=currentIndex){
						this->addLink(currentIndex,j,rand()%100);
					}
				}
			}
			
		}

		void addNodeWithFullLinks(int *costs){
			int currentIndex = this->addNode();
			for (int j = 0; j < this->graph.size(); j++)
			{
				if(j!=currentIndex){
					this->addLink(currentIndex,j,costs[j]);
				}
			}
		}

		void printGraph(){
			int i =0;
			for(auto& vec_of_nodes:this->graph){
				std::cout<<"node > "<< i++ <<'\n';
				for( auto& node: vec_of_nodes){
					std::cout<<"\t node > "<<node.node <<"|cost > "<<node.cost<<'\n';
				}
			}
		}
};


class SDL2Custom
{
private:
	SDL_Window *win;
	SDL_Renderer *rend;

	SDL_Window *win2;
	SDL_Renderer *rend2;

	bool success,running=true;
	std::vector<SDL_Rect> nodes;
	int clickedPosx,clickedPosy;

	bool notRendering = true;
	bool stillInserting=false;
	bool initWindow=false;
	bool destroyWindow=false;

public:
	SDL2Custom(/* args */);
	~SDL2Custom();

	void renderMessage(int posx, int posy,const char* m,int rect_w,int rect_h){

		TTF_Font* Sans = TTF_OpenFont("./Roboto-Medium.ttf", 12);
		if (Sans == NULL) {
			fprintf(stderr, "error: font not found\n");
			exit(EXIT_FAILURE);
   		 }
		SDL_Color White = {255, 255, 255,0};
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, m, White); 
		SDL_Texture* Message = SDL_CreateTextureFromSurface(this->rend, surfaceMessage);

		SDL_Rect Message_rect; //create a rect
		Message_rect.x = posx;  //controls the rect's x coordinate 
		Message_rect.y = posy; // controls the rect's y coordinte
		Message_rect.w = rect_w; // controls the width of the rect
		Message_rect.h = rect_h; // controls the height of the rect

		SDL_RenderCopy(this->rend, Message, NULL, &Message_rect);

		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
		TTF_CloseFont(Sans);
	}

	void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
	{
		const int32_t diameter = (radius * 2);

		int32_t x = (radius - 1);
		int32_t y = 0;
		int32_t tx = 1;
		int32_t ty = 1;
		int32_t error = (tx - diameter);

		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
	}

	node_t notInRect(int x , int y ){

		for (int i = 0; i < this->nodes.size(); i++)
		{
			SDL_Rect node =this->nodes[i]; 
			if((node.x - node.w/2<= x  &&  node.x + node.w + node.w/2 >= x ) && (node.y - node.h/2 )<=y && (node.y + node.h + node.h/2)>=y)
				return i;
		}
		return -1;
	}


	bool renderLoop(Graph* g){
		time_t c1,c2,c3;
		int totalCost;
		std::vector<node_t> nodesOfPath;
		int totalCost2;
		std::vector<node_t> nodesOfPath2;
		int dim=20;
		bool searched=false;

		int arr[100]={0};
		bool windowActive{true};
		if(!this->success)
			return false;
		while (running)
		{
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				
				ImGui_ImplSDL2_ProcessEvent(&e);
				switch (e.type)
				{
					case SDL_WINDOWEVENT:
							switch (e.window.event)
							{
								case SDL_WINDOWEVENT_CLOSE:
									this->running = false;
									break;
								
								default:
									break;
							}
						break;

					case SDL_KEYDOWN:
						switch (e.key.keysym.sym)
						{
							case SDLK_i:
								searched=true;
								
								c1 = clock();
								totalCost = g->findTSpath(nodesOfPath);
								c2 = clock();
								totalCost2 = g->findTSpath_direct(nodesOfPath2);
								c3 = clock();
								
								break;
							default:
								break;
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
							int xMouse, yMouse;
							SDL_GetMouseState(&xMouse,&yMouse);
							if(e.button.button == SDL_BUTTON_LEFT && this->notRendering){
								
								if(notInRect(xMouse,yMouse)==-1){
									this->stillInserting=true;
									this->initWindow=true;
									this->destroyWindow=false;
									this->notRendering=false;

									this->clickedPosx=xMouse;
									this->clickedPosy=yMouse;
									searched=false;
								}
								
							}else{

								std::cout<<"clicked\n";
							}
						break;
				}

				
			
				
			}


			SDL_SetRenderDrawColor(this->rend, (Uint8)(0), (Uint8)(0), (Uint8)(0), (Uint8)(255));
			SDL_RenderClear(this->rend);

			/**
			 * RENDER
			 * THE CIRCLES & LINKS PART
			 * 
			 */

		
			SDL_SetRenderDrawColor(this->rend,60, 179, 113,1);	
			
			//	render links
			for (int i = 0; i < this->nodes.size(); i++)
			{
				std::vector<NodeWithCost> currentNode = g->graph[i];
				// std::cout<<"size "<<currentNode.size()<<'\n';
				for (int j = 0; j < currentNode.size(); j++)
				{
					SDL_Rect rect1{this->nodes[i]},rect2{this->nodes[currentNode[j].node]};
					std::vector<node_t>::iterator ptr = std::find(nodesOfPath.begin(),nodesOfPath.end(),currentNode[j].node);
					
					bool isLinkOfPath=false;
					if(searched==true){
						if(		 *ptr==nodesOfPath.back()){
							isLinkOfPath = (*(nodesOfPath.begin())==i || *(ptr-1)==i)?true:false;
						}else if(ptr==nodesOfPath.begin()){
							isLinkOfPath = (*(ptr+1)==i || (nodesOfPath.back())==i)?true:false;
						}else{
							isLinkOfPath = (*(ptr+1)==i || *(ptr-1)==i)?true:false;
						}

						//	render the clock and the cost

						// std::cout<<"cost  : "<<totalCost<<'\n';
						// std::cout<<"cost2 : "<<totalCost2<<'\n';
						std::string mess1= "le cout est : " + std::to_string(totalCost);
						renderMessage(0,0,mess1.c_str(),100,50);

						std::stringstream s;
						s<<std::fixed<<std::setprecision(std::numeric_limits<double>::digits10)<<(double)(c2-c2)/CLOCKS_PER_SEC;
						std::string mess2= "t heuristique: " + s.str();
						
						// std::cout<<mess2<<'\n';
						renderMessage(0,380,mess2.c_str(),500,50);
						s.str("");
						s<<std::fixed<<std::setprecision(std::numeric_limits<double>::digits10)<<(double)(c3-c2)/CLOCKS_PER_SEC;
						std::string mess3= "t directe    : " + s.str();

						renderMessage(0,420,mess3.c_str(),500,50);
					}

					if(searched==true && isLinkOfPath){
						SDL_SetRenderDrawColor(this->rend,255, 0, 0,1);
					}else{
						SDL_SetRenderDrawColor(this->rend,60, 179, 113,1);	
					}

					SDL_RenderDrawLine(this->rend,rect1.x+rect1.w/2,rect1.y+rect1.h/2,rect2.x+rect2.w/2,rect2.y+rect2.h/2);
					this->renderMessage((rect1.x+rect2.x)/2,(rect1.y+rect2.y)/2,std::to_string(currentNode[j].cost).c_str(),15,15);
				}
			}

			if(searched!=true){
				SDL_SetRenderDrawColor(this->rend,60, 179, 113,1);	
			}else{
				SDL_SetRenderDrawColor(this->rend,255, 0, 0,1);
			}

			for (int i = 0; i < this->nodes.size(); i++)
			{
				SDL_Rect node = this->nodes[i];

				SDL_SetRenderDrawColor(this->rend,60, 179, 113,1);
				DrawCircle(this->rend,node.x + node.w/2,node.y + node.h/2,dim);
				
				std::string s(1, 'A' + i);
				this->renderMessage(node.x+node.w/4,node.y + node.h/4,s.c_str(),15,15);
			
			}



			/**
			 * IMGUI SHIT 
			 * 
			 */

			bool states[100];

			if (this->stillInserting)
			{

				ImGui_ImplSDLRenderer_NewFrame();
				ImGui_ImplSDL2_NewFrame();
				ImGui::NewFrame();

				//	give the cost prompt
				ImGui::Begin("couts noeuds",&windowActive,ImGuiWindowFlags_AlwaysAutoResize );
				for (int i = 0; i < g->graph.size(); i++)
				{
					char c = 'A'+i;
					char s[4];
					s[0]=c;s[1]='\0';
					states[i] = ImGui::InputInt(s,&arr[i]);
				}

				if(ImGui::Button("fini")){
					//	we're done with the texte
					this->stillInserting=false;
					this->destroyWindow=true;
					std::cout<<"finished\n";

				}

				ImGui::End();
				if(!this->stillInserting)
					ImGui::EndFrame();


			}

			if (this->destroyWindow)
			{

				g->addNodeWithFullLinks(arr);
				this->nodes.push_back((SDL_Rect){(clickedPosx) - dim, (clickedPosy) - dim,dim*2,dim*2});
				std::fill_n(arr, 100, 0);

				this->destroyWindow=false;
				this->notRendering=true;

				std::cout<<"done\n";
				g->printGraph();
				
			}

			if(!this->notRendering){

				ImGui::Render();

				ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
			}

			SDL_RenderPresent(this->rend);
		}
		
		return true;
	}
	private:

		void initSubWindowAndRend_imgui(void){
		
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
  		 	ImGui::StyleColorsDark();

			ImGui_ImplSDL2_InitForSDLRenderer(this->win, this->rend);
			ImGui_ImplSDLRenderer_Init(this->rend);
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		}

		void destroySubWinAndRend_imgui(void){

			ImGui_ImplSDLRenderer_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
			SDL_DestroyRenderer(this->rend);
			SDL_DestroyWindow(this->win);
		}
};

SDL2Custom::SDL2Custom(/* args */)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	this->win = SDL_CreateWindow(
		"TP2 - points d'articulations",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	if (this->win == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		this->success = false;
	}else{
		this->success=true;
	}

	this->rend = SDL_CreateRenderer(this->win, -1, SDL_RENDERER_SOFTWARE);
	TTF_Init();
	this->initSubWindowAndRend_imgui();

}

SDL2Custom::~SDL2Custom()
{
	destroySubWinAndRend_imgui();
	TTF_Quit();
	SDL_DestroyRenderer(this->rend);
	SDL_DestroyWindow(this->win);
	
	SDL_Quit();
}



int main(void){
	Graph* g = new Graph();


	SDL2Custom s = SDL2Custom();
	s.renderLoop(g);

	return 0;
}