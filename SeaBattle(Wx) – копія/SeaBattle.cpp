#include "wx/wx.h"
#include "wx/notebook.h"
#include "Functions.h"

#include <vector>
#include<thread>
#include<chrono>

using namespace std;

class Main_window;

class Start_panel : public wxPanel
{
public:

	wxButton* start_B = new wxButton(this, 101, "Start", wxDefaultPosition, wxSize(300, 75));
	wxButton* options_B = new wxButton(this, 102, "Options", wxDefaultPosition, wxSize(150, 25));
	wxButton* exit_B = new wxButton(this, 103, "Exit", wxDefaultPosition, wxSize(150, 25));

	Start_panel(wxFrame* parent, int id = wxID_ANY) : wxPanel(parent, id)
	{
		wxSize screenSize = wxGetDisplaySize();

		SetPosition(wxPoint((screenSize.GetWidth() - GetSize().GetWidth()) / 2, (screenSize.GetHeight() - GetSize().GetHeight()) / 2));

		this->SetBackgroundColour(wxColour(0x23, 0x45, 0x67));
		this->SetMinSize(wxSize(480, 400));

		//----- Top Sizer -----

		wxFont font1(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		start_B->SetFont(font1);
		start_B->SetMaxSize(wxSize(1920, 75));
		start_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		wxBoxSizer* sizer_top = new wxBoxSizer(wxVERTICAL);
		sizer_top->Add(start_B, 1, wxEXPAND);

		//----- Top Sizer -----

		//----- Bottom Sizer -----
		wxFont font2(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		options_B->SetFont(font2);
		options_B->SetMaxSize(wxSize(1920, 75));
		options_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		exit_B->SetFont(font2);
		exit_B->SetMaxSize(wxSize(1920, 75));
		exit_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
		sizer_bottom->Add(options_B, 1, wxALIGN_BOTTOM);
		sizer_bottom->Add(exit_B, 1, wxALIGN_BOTTOM);

		//----- Bottom Sizer -----

		wxBoxSizer* sizer_general = new wxBoxSizer(wxVERTICAL);

		sizer_general->Add(sizer_top, 1, wxEXPAND);
		sizer_general->Add(sizer_bottom, 1, wxEXPAND);

		this->SetSizer(sizer_general);
	}
};

class Options_window : public wxFrame
{
public:

	wxButton* save_B;
	wxButton* cancel_B;
	wxCheckBox* isFullscreen;
	wxCheckBox* isInstantMove;

	wxBoxSizer* sizer_top;
	wxBoxSizer* sizer_general;

	//game variables
	int AI_FeedbackSpeed = 1;

	//options old variables
	bool old_isFullscreen = false;
	bool old_isInstantMove = false;

	Options_window(wxFrame* parent) : wxFrame(nullptr, 700, "Options", wxDefaultPosition, wxSize(360, 300))
	{
		this->SetBackgroundColour(wxColour(0x23, 0x45, 0x67));
		this->SetMinSize(wxSize(120, 100));

		this->SetPosition(GetWindowCenter(parent, this));

		sizer_top = new wxBoxSizer(wxVERTICAL);
		sizer_general = new wxBoxSizer(wxVERTICAL);

		save_B = new wxButton(this, 701, "Save changes");
		save_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
		//save_B->SetMaxSize(wxSize(1920, 75));

		cancel_B = new wxButton(this, 702, "Cancel");
		cancel_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		isFullscreen = new wxCheckBox(this, 703, "Fullscreen");
		isInstantMove = new wxCheckBox(this, 704, "Isntant AI move");

		sizer_top->Add(isFullscreen, wxSizerFlags().Center().Border(wxTOP, 10));
		sizer_top->Add(isInstantMove, wxSizerFlags().Center().Border(wxTOP, 10));

		sizer_general->Add(sizer_top, 5, wxEXPAND);
		sizer_general->Add(cancel_B, 1, wxEXPAND);
		sizer_general->Add(save_B, 1, wxEXPAND);

		this->SetSizer(sizer_general);

		//-------- Bindings -------------------------------

		this->Bind(wxEVT_CLOSE_WINDOW, &Options_window::OnClose, this);

		cancel_B->Bind(wxEVT_BUTTON, &Options_window::Exit_window, this, cancel_B->GetId());
		save_B->Bind(wxEVT_BUTTON, &Options_window::Exit_window, this, save_B->GetId());

		//-------- Bindings -------------------------------

	}

	void OnClose(wxCloseEvent& evt)
	{
		Hide();
		evt.Skip();
	}

	void Exit_window(wxCommandEvent& evt)
	{
		isFullscreen->SetValue(old_isFullscreen);
		isInstantMove->SetValue(old_isInstantMove);

		Hide();
		evt.Skip();
	}
};

class Pre_begin_panel : public wxPanel
{
public:

	bool isTwoPlayers = false;

	wxButton* start_B = new wxButton(this, 208, "Start the game");
	wxButton* prev_window_B = new wxButton(this, 209, "Back", wxDefaultPosition, wxSize(70, 35));

	wxStaticText* str_mapSize = new wxStaticText(this, wxID_ANY, "Choose map size");;
	wxStaticText* str_difficulty = new wxStaticText(this, wxID_ANY, "Choose difficulty");
	wxStaticText* errorbox = new wxStaticText(this, wxID_ANY, "");;

	vector<wxRadioButton*> rad_map_size =
	{
		new wxRadioButton(this, 201, "Small (10x10)"),
		new wxRadioButton(this, 202, "Meduim (15x15)"),
		new wxRadioButton(this, 203, "Big (20x20)"),
	};

	vector<wxRadioButton*> rad_difficulty =
	{
		new wxRadioButton(this, 204, "Two players"),
		new wxRadioButton(this, 205, "Easy (AI)"),
		new wxRadioButton(this, 206, "Meduim (AI)"),
		new wxRadioButton(this, 207, "Hard (AI)"),
	};

	Pre_begin_panel(wxFrame* parent, int id = wxID_ANY) : wxPanel(parent, id)
	{
		this->SetBackgroundColour(wxColour(0x23, 0x45, 0x67));
		this->SetMinSize(wxSize(480, 400));

		wxFont header_font(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		wxBoxSizer* sizer_general = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* sizer_top = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* sizer_left = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* sizer_right = new wxBoxSizer(wxVERTICAL);

		str_mapSize->SetFont(header_font);

		rad_map_size[0]->SetWindowStyleFlag(wxRB_GROUP);

		sizer_left->Add(str_mapSize, 3, wxALIGN_CENTER | wxTOP, 25);
		sizer_left->Add(rad_map_size[0], 2, wxALIGN_CENTER);
		sizer_left->Add(rad_map_size[1], 2, wxALIGN_CENTER);
		sizer_left->Add(rad_map_size[2], 2, wxALIGN_CENTER);

		str_difficulty->SetFont(header_font);

		rad_difficulty[0]->SetWindowStyleFlag(wxRB_GROUP);
		sizer_right->Add(str_difficulty, 3, wxALIGN_CENTER | wxTOP, 25);
		sizer_right->Add(rad_difficulty[0], 2, wxALIGN_CENTER);
		sizer_right->Add(rad_difficulty[1], 2, wxALIGN_CENTER);
		sizer_right->Add(rad_difficulty[2], 2, wxALIGN_CENTER);
		sizer_right->Add(rad_difficulty[3], 2, wxALIGN_CENTER);

		sizer_top->Add(sizer_left, 1, wxEXPAND);
		sizer_top->Add(sizer_right, 1, wxEXPAND);

		
		start_B->SetFont(header_font);
		start_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
		start_B->SetMaxSize(wxSize(1920, 75));

		
		prev_window_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		sizer_general->Add(prev_window_B, 0, wxALIGN_LEFT);
		sizer_general->Add(sizer_top, 4, wxEXPAND, 10);
		sizer_general->Add(errorbox, 1, wxEXPAND);
		sizer_general->Add(start_B, 1, wxEXPAND);

		this->SetSizer(sizer_general);
	}

	int GetMapSize()
	{
		for (int i = 0; i < rad_map_size.size(); i++)
		{
			if (rad_map_size[i]->GetValue())
			{
				return 5 + 5 * (i + 1);
				break;
			}
		}
		return -1;
	}

	int GetDifficulty()
	{
		for (int i = 0; i < rad_difficulty.size(); i++)
		{
			if (rad_difficulty[i]->GetValue())
			{
				return i;
				break;
			}
		}
		return -1;
	}
};

class Layout_panel : public wxPanel
{
public: 
	string first_B_name;
	
	wxPanel* first_player = new wxPanel(this, wxID_ANY);
	wxButton* first_back_B = new wxButton(first_player, 301, "Back");
	wxButton* first_B;
	wxButton* first_random_B = new wxButton(first_player, 303, "Arrange randomly");

	wxPanel* second_player = new wxPanel(this, wxID_ANY);
	wxButton* second_back_B = new wxButton(second_player, 351, "Back");
	wxButton* second_B;
	wxButton* second_random_B = new wxButton(second_player, 353, "Arrange randomly");

	vector<vector<vector<wxButton*>>> arrangement_map;
	vector<vector<vector<int>>> ship_map;
	int map_size;
	bool isTwoPlayers;

	Layout_panel(wxFrame* parent, short map_size, bool isTwoPlayers, int id = wxID_ANY) : wxPanel(parent, id)
	{
		this->SetBackgroundColour(wxColour(0x23, 0x45, 0x67));
		this->SetMinSize(wxSize(480, 400));

		this->map_size = map_size;
		this->isTwoPlayers = isTwoPlayers;

		isTwoPlayers ? first_B_name = "Next" : first_B_name = "Finish";
		first_B = new wxButton(first_player, 302, first_B_name);

		wxBoxSizer* sizer_general = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer* sizer_first = new wxBoxSizer(wxHORIZONTAL);
		fill_map(map_size, first_player);
		wxGridSizer* first_map = new wxGridSizer(map_size, map_size, 0, 0);

		wxBoxSizer* sizer_right_first = new wxBoxSizer(wxVERTICAL);

		first_back_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
		first_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
		first_random_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		for (int i = 0; i < arrangement_map[0].size(); i++)
		{
			for (int j = 0; j < arrangement_map[0][i].size(); j++)
			{
				first_map->Add(arrangement_map[0][i][j], 1, wxEXPAND);
			}
		}
		sizer_right_first->Add(first_back_B, 1, wxEXPAND );
		sizer_right_first->Add(first_random_B, 1, wxEXPAND);
		sizer_right_first->Add(first_B, 1, wxEXPAND);

		sizer_first->Add(first_map, 5, wxEXPAND);
		sizer_first->Add(sizer_right_first, 1, wxEXPAND);

		first_player->SetSizer(sizer_first);
		sizer_general->Add(first_player, 1, wxEXPAND);

		//Bindings

		//Bindings

		if(isTwoPlayers)
		{
			second_B = new wxButton(second_player, 352, "Finish");

			wxBoxSizer* sizer_second = new wxBoxSizer(wxHORIZONTAL);
			fill_map(map_size, second_player, isTwoPlayers);
			wxGridSizer* second_map = new wxGridSizer(map_size, map_size, 0, 0);

			wxBoxSizer* sizer_right_second = new wxBoxSizer(wxVERTICAL);

			second_back_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
			second_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
			second_random_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

			for (int i = 0; i < arrangement_map[1].size(); i++)
			{
				for (int j = 0; j < arrangement_map[1][i].size(); j++)
				{
					second_map->Add(arrangement_map[1][i][j], 1, wxEXPAND);
				}
			}
			sizer_right_second->Add(second_back_B, 1, wxEXPAND);
			sizer_right_second->Add(second_random_B, 1, wxEXPAND);
			sizer_right_second->Add(second_B, 1, wxEXPAND);

			sizer_second->Add(second_map, 5, wxEXPAND);
			sizer_second->Add(sizer_right_second, 1, wxEXPAND);

			second_player->SetSizer(sizer_second);
			second_player->Hide();
			sizer_general->Add(second_player, 1, wxEXPAND);
		}

		this->SetSizer(sizer_general);
	}

	void fill_map(short map_size, wxPanel* parent, bool isSecond = false)
	{
		wxFont font1(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		vector<wxButton*> vec_temp;
		vector<vector<wxButton*>> matrix_temp;
		wxButton* button_temp;

		int s;
		isSecond ? s = 1000 : s = 0;

		for (int i = 0; i < map_size; i++)
		{
			vec_temp.clear();
			for (int j = 0; j < map_size; j++)
			{
				button_temp = new wxButton(parent, 4001 + j + i * 100 + s, "O", wxDefaultPosition, wxSize(35, 35));
				button_temp->SetFont(font1);
				button_temp->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

				vec_temp.push_back(button_temp);
			}
			matrix_temp.push_back(vec_temp);
		}
		arrangement_map.push_back(matrix_temp);
	}
};

class Game_panel : public wxPanel
{
	wxBoxSizer* sizer_general;

	wxGridSizer* Player_grid[2];

	wxBoxSizer* sizer_player[2];
	wxBoxSizer* sizer_top[2];
	wxBoxSizer* sizer_bottom[2];

	wxStaticText* label_player1;
	wxStaticText* label_player2;

public:

	vector<vector<wxButton*>> game_map[2];

	wxButton* pause_B;

	vector<wxRadioBox*> rad_shoot_type;

	int shoot_type = 0;

	Ammunition ammunition[2];
	wxString choices1[3] = { "regular : infinity", "Big (3x3)", "Torpedo"};

	Game_panel(wxFrame* parent, int map_size, int difficulty, int id = wxID_ANY) : wxPanel(parent, id)
	{
		this->SetBackgroundColour(wxColour(0x23, 0x45, 0x67));

		sizer_general = new wxBoxSizer(wxHORIZONTAL);

		sizer_top[0] = new wxBoxSizer(wxHORIZONTAL);
		sizer_top[1] = new wxBoxSizer(wxHORIZONTAL);

		Player_grid[0] = new wxGridSizer(map_size, map_size, 2, 2);
		Player_grid[1] = new wxGridSizer(map_size, map_size, 2, 2);
		
		sizer_bottom[0] = new wxBoxSizer(wxHORIZONTAL);
		sizer_bottom[1] = new wxBoxSizer(wxHORIZONTAL);

		sizer_player[0] = new wxBoxSizer(wxVERTICAL);
		sizer_player[1] = new wxBoxSizer(wxVERTICAL);

		//--- Top Sizers ----------------------------------------------------

		sizer_top[0] = new wxBoxSizer(wxHORIZONTAL);
		sizer_top[1] = new wxBoxSizer(wxHORIZONTAL);

		wxFont font1(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		label_player1 = new wxStaticText(this, 399, "Player 1");
		label_player2 = new wxStaticText(this, 399, "Player 2");
		label_player1->SetFont(font1);
		label_player2->SetFont(font1);
		label_player1->SetWindowStyle(wxALIGN_CENTER);
		label_player2->SetWindowStyle(wxALIGN_CENTER);

		pause_B = new wxButton(this, 4051, "Pause", wxPoint(5, 5), wxSize(70, 35));
		pause_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		sizer_top[0]->Add(pause_B, 0);
		sizer_top[0]->Add(label_player1, 1, wxALIGN_CENTER);
		sizer_top[1]->Add(label_player2, 1, wxALIGN_CENTER);

		sizer_player[0]->Add(sizer_top[0], 1, wxEXPAND);
		sizer_player[1]->Add(sizer_top[1], 1, wxEXPAND);

		//--- Top  Sizers ----------------------------------------------------

		//--- Grid Sizers ----------------------------------------------------

		vector<wxButton*> vec_temp;
		wxButton* button_temp;

		for (int i = 0; i < map_size; i++)
		{
			vec_temp.clear();
			for (int j = 0; j < map_size; j++)
			{
				button_temp = new wxButton(this, 10000 + j + i * 100, "O", wxDefaultPosition, wxSize(35, 35));
				button_temp->SetFont(font1);
				button_temp->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

				Player_grid[0]->Add(button_temp, 1, wxEXPAND);

				vec_temp.push_back(button_temp);
			}
			game_map[0].push_back(vec_temp);
		}
		for (int i = 0; i < map_size; i++)
		{
			vec_temp.clear();
			for (int j = 0; j < map_size; j++)
			{
				button_temp = new wxButton(this, 20000 + j + i * 100, "O", wxDefaultPosition, wxSize(35, 35));
				button_temp->SetFont(font1);
				button_temp->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

				Player_grid[1]->Add(button_temp, 1, wxEXPAND);

				vec_temp.push_back(button_temp);
			}
			game_map[1].push_back(vec_temp);
		}

		sizer_player[0]->Add(Player_grid[0], 8, wxEXPAND);
		sizer_player[1]->Add(Player_grid[1], 8, wxEXPAND);

		Player_grid[0]->Layout();
		Player_grid[1]->Layout();

		//--- Grid Sizers ----------------------------------------------------

		//--- Bottom  Sizers ----------------------------------------------------

		rad_shoot_type.push_back(new wxRadioBox(this, wxID_ANY, "player 1", wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices1), choices1, 1, wxRA_SPECIFY_ROWS));
		rad_shoot_type.push_back(new wxRadioBox(this, wxID_ANY, "player 2", wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices1), choices1, 1, wxRA_SPECIFY_ROWS));

		sizer_bottom[0]->Add(rad_shoot_type[0], 1, wxEXPAND);
		sizer_bottom[1]->Add(rad_shoot_type[1], 1, wxEXPAND);

		sizer_player[0]->Add(sizer_bottom[0], 1, wxEXPAND);
		sizer_player[1]->Add(sizer_bottom[1], 1, wxEXPAND);

		//--- Bottom Sizers ----------------------------------------------------

		//--- General Sizer ----------------------------------------------------

		this->SetMinSize(wxSize((40 + 35 * map_size) * 2, 100));

		sizer_general->Add(sizer_player[0], 1, wxEXPAND | wxALL, 10);
		sizer_general->Add(sizer_player[1], 1, wxEXPAND | wxALL, 10);

		this->SetSizerAndFit(sizer_general);
		//--- General Sizer ----------------------------------------------------
	}

	int getShootType(int map_idx)
	{
		return rad_shoot_type[map_idx]->GetSelection() + 1;
	}
};

class Pause_window : public wxFrame
{
public:

	wxButton* resume_B;
	wxButton* options_B;
	wxButton* quit_B;

	wxBoxSizer* sizer_general;

	Options_window* options_window;

	Pause_window(wxFrame* parent) : wxFrame(nullptr, 600, "Pause", wxDefaultPosition, wxSize(360, 300))
	{
		this->SetBackgroundColour(wxColour(0x23, 0x45, 0x67));
		this->SetMinSize(wxSize(240, 200));

		this->SetPosition(GetWindowCenter(parent, this));

		resume_B = new wxButton(this, 601, "Resume", wxDefaultPosition, wxSize(100, 50));
		options_B = new wxButton(this, 602, "Options", wxDefaultPosition, wxSize(100, 50));
		quit_B = new wxButton(this, 603, "Quit", wxDefaultPosition, wxSize(100, 50));

		resume_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
		options_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));
		quit_B->SetBackgroundColour(wxColor(0x45, 0x67, 0x89));

		resume_B->SetMaxSize(wxSize(1920 * .8, 100));
		options_B->SetMaxSize(wxSize(1920 * .8, 100));
		quit_B->SetMaxSize(wxSize(1920 * .8, 100));

		//--- General Sizer ----------------------------------------------------

		sizer_general = new wxBoxSizer(wxVERTICAL);
		sizer_general->AddSpacer(15);

		sizer_general->Add(resume_B, 1, wxEXPAND);
		sizer_general->Add(options_B, 1, wxEXPAND);
		sizer_general->Add(quit_B, 1, wxEXPAND);

		//--- General Sizer ----------------------------------------------------

		this->SetSizer(sizer_general);

		//-------- Bindings -------------------------------

		this->Bind(wxEVT_CLOSE_WINDOW, &Pause_window::OnClose, this);

		resume_B->Bind(wxEVT_BUTTON, &Pause_window::Exit_window, this, resume_B->GetId());

		//-------- Bindings -------------------------------

	}

	~Pause_window()
	{

	}

	void Exit_window(wxCommandEvent& evt)
	{

		//continue time

		Hide();
		evt.Skip();
	}

	void OnClose(wxCloseEvent& evt)
	{
		//continue time

		Hide();
		evt.Skip();
	}

};

class Main_window : public wxFrame
{
	Start_panel* start_panel;
	Pre_begin_panel* pre_begin_panel;
	Layout_panel* layout_panel;
	Game_panel* game_panel;
	Pause_window* pause_window = new Pause_window(this);
	Options_window* options_window = new Options_window(this);

	wxSize screenSize = wxGetDisplaySize();
	wxBoxSizer* sizer_general;

	bool player1_else_player2 = true;
	bool botMove = false;

public:
	Main_window() : wxFrame(nullptr, wxID_ANY, "Battleship", wxDefaultPosition, wxSize(1680, 960))
	{

		this->SetMinSize(wxSize(480, 400));

		this->Centre();

		start_panel = new Start_panel(this);
		pre_begin_panel = new Pre_begin_panel(this);

		sizer_general = new wxBoxSizer(wxVERTICAL);

		sizer_general->Add(start_panel, 1, wxEXPAND);
		sizer_general->Add(pre_begin_panel, 1, wxEXPAND);

		pre_begin_panel->Hide();

		this->SetSizerAndFit(sizer_general);
		
		//-------- Bindings -------------------------------

		this->Bind(wxEVT_CLOSE_WINDOW, &Main_window::OnClose, this);

		//start_panel bindings
		start_panel->start_B->Bind(wxEVT_BUTTON, &Main_window::Close_start_panel_Open_pre_begin_panel, this, start_panel->start_B->GetId());
		start_panel->options_B->Bind(wxEVT_BUTTON, &Main_window::Open_options_panel, this, start_panel->options_B->GetId());
		start_panel->exit_B->Bind(wxEVT_BUTTON, &Main_window::Exit_App, this, start_panel->exit_B->GetId());

		//pre_begin_panel bindings
		pre_begin_panel->start_B->Bind(wxEVT_BUTTON, &Main_window::Close_pre_begin_panel_Open_layout_panel, this, pre_begin_panel->start_B->GetId());
		pre_begin_panel->prev_window_B->Bind(wxEVT_BUTTON, &Main_window::Close_pre_begin_panel_Open_start_panel, this, pre_begin_panel->prev_window_B->GetId());

		//layout_panel bindings
		//inside Close_pre_begin_panel_Open_layout_panel()

		//game_panel bindings
		//inside Close_layout_panel_Open_game_panel()

		//pause_window bindings
		pause_window->options_B->Bind(wxEVT_BUTTON, &Main_window::Open_options_panel, this, pause_window->options_B->GetId());
		pause_window->quit_B->Bind(wxEVT_BUTTON, &Main_window::Close_game_panel_Open_start_panel, this, pause_window->quit_B->GetId());

		//options_window bindings
		options_window->save_B->Bind(wxEVT_BUTTON, &Main_window::SaveChanges, this, options_window->save_B->GetId());
		
		//-------- Bindings -------------------------------
	}

	void Update_window()
	{
		this->SetSize(GetSize().x + 1, GetSize().y + 1);
		this->SetSize(GetSize().x - 1, GetSize().y - 1);
	}

	//----------  Exit App Functions ----------

	void OnClose(wxCloseEvent& evt)
	{
		options_window->Destroy();
		pause_window->Destroy();

		Destroy();
		evt.Skip();
	}

	void Exit_App(wxCommandEvent& evt)
	{
		options_window->Close();
		pause_window->Close();

		Close();
		evt.Skip();
	}

	//----------  Exit App Functions ----------

	//---------- Button Click Functions ----------

	void Close_start_panel_Open_pre_begin_panel(wxCommandEvent& evt);
	void Close_pre_begin_panel_Open_start_panel(wxCommandEvent& evt);
	void Close_pre_begin_panel_Open_layout_panel(wxCommandEvent& evt);
	void Close_layout_panel_Open_pre_begin_panel(wxCommandEvent& evt);
	void Close_layout_panel_Open_game_panel(wxCommandEvent& evt);
	void Close_game_panel_Open_start_panel(wxCommandEvent& evt);
	void Open_pause_panel(wxCommandEvent& evt);
	void Open_options_panel(wxCommandEvent& evt);

	void On_layout_panel_next(wxCommandEvent& evt);
	void On_layout_panel_prev(wxCommandEvent& evt);

	void SaveChanges(wxCommandEvent& evt)
	{
		options_window->Hide();

		//save changes
		this->ShowFullScreen(options_window->isFullscreen->GetValue());
		options_window->AI_FeedbackSpeed = options_window->isInstantMove->GetValue() ? 0 : 1;
		//save cghanges

		options_window->old_isFullscreen = options_window->isFullscreen->GetValue();
		options_window->old_isInstantMove = options_window->isInstantMove->GetValue();

		evt.Skip();
	}

	//---------- Button Click Functions ----------

	//---------- Random Fill Functions ----------

	bool is_random_colision(vector<vector<int>> field, int x, int y)
	{
		if ((x - 1) > 0 && (y - 1) > 0 && (x + 1) < layout_panel->map_size + 1 && (y + 1) < layout_panel->map_size + 1)
		{
			if (field[x + 1][y] == 0 && field[x][y + 1] == 0 && field[x + 1][y + 1] == 0)
			{
				if (field[x - 1][y] == 0 && field[x][y - 1] == 0 && field[x - 1][y - 1] == 0)
				{
					if (field[x + 1][y - 1] == 0 && field[x - 1][y + 1] == 0)
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	void call_random_fill(wxCommandEvent& evt)
	{
		random_fill_map();

		if (pre_begin_panel->GetDifficulty()) // if one player
			random_fill_map();

		evt.Skip();
	}

	void random_fill_map()
	{
		srand(time(NULL));

		vector<vector<int>> field; //only for random_fiil_map() use
		vector<int> temp;
		for (int i = 0; i < layout_panel->map_size + 2; i++)
		{
			temp.clear();
			for (int j = 0; j < layout_panel->map_size + 2; j++)
				temp.push_back(0);
			field.push_back(temp);
		}

		int x = 0, y = 0, direction = 0, deck, ship_count;
		switch (layout_panel->map_size)
		{
		case 10:
		{
			deck = 4;
			ship_count = 1;
			break;
		}
		case 15:
		{
			deck = 5;
			ship_count = 2;
			break;
		}
		case 20:
		{
			deck = 5;
			ship_count = 3;
			break;
		}
		}

		for (; deck > 0; deck--)
		{
			for (int i = 0; i < ship_count; i++)
			{

			Again:

				direction = rand() % 4 + 1;
				x = rand() % layout_panel->map_size + 1;
				y = rand() % layout_panel->map_size + 1;

				switch (direction)
				{
				case 1:
				{

					for (int i = 0; i < deck; i++)
					{
						if (is_random_colision(field, x, y + i))
						{
							goto Again;
						}
					}

					for (int i = 0; i < deck; i++)
					{
						field[x][y + i] = 1;
					}
					break;
				}
				case 2:
				{
					for (int i = 0; i < deck; i++)
					{
						if (is_random_colision(field, x, y - i))
						{
							goto Again;
						}
					}

					for (int i = 0; i < deck; i++)
					{
						field[x][y - i] = 1;
					}
					break;
				}
				case 3:
				{
					for (int i = 0; i < deck; i++)
					{
						if (is_random_colision(field, x - i, y))
						{
							goto Again;
						}
					}

					for (int i = 0; i < deck; i++)
					{
						field[x - i][y] = 1;
					}
					break;
				}
				case 4:
				{
					for (int i = 0; i < deck; i++)
					{
						if (is_random_colision(field, x + i, y))
						{
							goto Again;
						}
					}

					for (int i = 0; i < deck; i++)
					{
						field[x + i][y] = 1;
					}
					break;
				}
				}
			}
			ship_count++;
		}

		vector<vector<int>> result;
		for (int i = 1; i < layout_panel->map_size + 1; i++)
		{
			temp.clear();
			for (int j = 1; j < layout_panel->map_size + 1; j++)
			{
				temp.push_back(field[i][j]);
			}
			result.push_back(temp);
		}

		layout_panel->ship_map.push_back(result);

		//Connect arrangement_map matrix and ship_map matrix
	}

	//---------- Random Fill Functions ----------

	//---------- Gameplay Functions ----------

	void make_move(wxCommandEvent& evt)
	{
		switch (game_panel->getShootType(player1_else_player2 ? 1 : 0))
		{
		case 2:
		{
			if (game_panel->ammunition[player1_else_player2 ? 1 : 0].big_ammo_count > 0)
			{
				game_panel->ammunition[player1_else_player2 ? 1 : 0].big_ammo_count--;
				game_panel->rad_shoot_type[player1_else_player2 ? 1 : 0]->SetString(1, "Big (3x3) : " + toString(game_panel->ammunition[player1_else_player2 ? 1 : 0].big_ammo_count) + " left");
			}
			else
			{
				wxMessageBox("There`s no such ammo left", "Out of ammo", wxOK | wxICON_INFORMATION);
				return;
			}
			break;
		}
		case 3:
		{
			if (game_panel->ammunition[player1_else_player2 ? 1 : 0].torpedo_count > 0)
			{
				game_panel->ammunition[player1_else_player2 ? 1 : 0].torpedo_count--;
				game_panel->rad_shoot_type[player1_else_player2 ? 1 : 0]->SetString(2, "Torpedo : " + toString(game_panel->ammunition[player1_else_player2 ? 1 : 0].torpedo_count) + " left");
			}
			else
			{
				wxMessageBox("There`s no such ammo left", "Out of ammo", wxOK | wxICON_INFORMATION);
				return;
			}
			break;
		}
		}

		int id = evt.GetId();

		int j = (id % 100) % layout_panel->map_size;
		int i = (id % 10000) / 100;

		//continue here---------------------------------------------------------------------------------------------------------------------------------------------------------------

		if (!shoot(i, j, player1_else_player2 ? 0 : 1)) // if hit = false
		{
			this->disableButtons();
			botMove = true;
		}
		else
		{
			game_panel->game_map[player1_else_player2 ? 0 : 1][i][j]->Disable();
			if (isMapEmpty(layout_panel->ship_map[player1_else_player2 ? 0 : 1]))
				endGame(player1_else_player2 ? 0 : 1);
		}

		if (pre_begin_panel->GetDifficulty() && botMove) // If player vs bot and now it`s bot`s turn
		{
		shootAgain:

			//---------------------------- here

			std::this_thread::sleep_for(std::chrono::seconds(options_window->AI_FeedbackSpeed));

			int bi, bj;
			//function of bot difficulty
			switch (pre_begin_panel->GetDifficulty())
			{
			case 1:
			{
				bot_shoot_easy(bi, bj);
				break;
			}
			case 2:
			{
				bot_shoot_medium(bi, bj, layout_panel->ship_map[0]);
				break;
			}
			case 3:
			{
				bot_shoot_hard(bi, bj);
				break;
			}
			}
			//function of bot difficulty

			if (!shoot(bi, bj, 0)) // if hit = false
			{
				this->disableButtons();
				botMove = false;
			}
			else
			{
				game_panel->game_map[0][i][j]->Disable();
				if (isMapEmpty(layout_panel->ship_map[0]))
					endGame(0);
				goto shootAgain;
			}

		}

		evt.Skip();
	}

	int isMapEmpty(vector<vector<int>> map)
	{

		for (int i = 0; i < map.size(); i++)
		{
			for (int j = 0; j < map[i].size(); j++)
			{
				if (map[i][j] == 1)
					return false;
			}
		}
		return true;
	}

	void endGame(int res)
	{
		string massage;
		switch (res)
		{
		case 0:
		{
			if (pre_begin_panel->GetDifficulty())
				massage = "You lost";
			else
				massage = "Player 2 won";
			break;
		}
		case 1:
		{
			if (pre_begin_panel->GetDifficulty())
				massage = "You won!";
			else
				massage = "Player 1 won";
			break;
		}
		}
		wxMessageBox(massage, "Game has ended", wxOK | wxICON_INFORMATION);

		game_panel->Hide();
		start_panel->Show();
		Update_window();

		player1_else_player2 = true;
		botMove = false;
	}

	bool shoot(int x, int y, int map_idx)
	{
		bool isHit = false;
		switch (game_panel->getShootType(!map_idx))
		{
		case 1:
		{
			isHit = regularShoot(x, y, map_idx);
			break;
		}
		case 2:
		{
			isHit = bigShoot(x, y, map_idx);
			break;
		}
		case 3:
		{
			isHit = rowShoot(x, y, map_idx);
			break;
		}
		}

		return isHit;
	}

	void disableButtons()
	{
		if (player1_else_player2)
		{
			for (int i = 0; i < game_panel->game_map[0].size(); i++)
			{
				for (int j = 0; j < game_panel->game_map[0][i].size(); j++)
				{
					game_panel->game_map[0][i][j]->Disable();

					if (layout_panel->ship_map[1][i][j] >= 0)
						game_panel->game_map[1][i][j]->Enable();
				}
			}
		}
		else
		{
			for (int i = 0; i < game_panel->game_map[0].size(); i++)
			{
				for (int j = 0; j < game_panel->game_map[0][i].size(); j++)
				{
					game_panel->game_map[1][i][j]->Disable();
				
					if (layout_panel->ship_map[0][i][j] >= 0)
						game_panel->game_map[0][i][j]->Enable();
				}
			}
		}

		player1_else_player2 = !player1_else_player2;
		botMove = !botMove;
	}

	//---------- Gameplay Functions ----------

	//---------- Bot Difficulty ----------

	void bot_shoot_easy(int& x, int& y)
	{
		srand(time(NULL));

		int random_choice = rand() % 101;
		if (random_choice >= 0 && random_choice <= 75)
			game_panel->rad_shoot_type[1]->SetSelection(0);
		else if (game_panel->ammunition[1].big_ammo_count > 0 && random_choice >= 76 && random_choice <= 90)
		{
			game_panel->ammunition[1].big_ammo_count--;
			game_panel->rad_shoot_type[1]->SetString(1, "Big (3x3) : " + toString(game_panel->ammunition[1].big_ammo_count) + " left");

			game_panel->rad_shoot_type[1]->SetSelection(1);
		}
		else if (game_panel->ammunition[1].torpedo_count > 0 && random_choice >= 91 && random_choice <= 100)
		{
			game_panel->ammunition[1].torpedo_count--;
			game_panel->rad_shoot_type[1]->SetString(2, "Torpedo : " + toString(game_panel->ammunition[1].torpedo_count) + " left");

			game_panel->rad_shoot_type[1]->SetSelection(2);
		}

		/*switch (game_panel->getShootType(player1_else_player2 ? 1 : 0))
		{
		case 2:
		{
			if (game_panel->ammunition[player1_else_player2 ? 1 : 0].big_ammo_count > 0)
			{
				game_panel->ammunition[player1_else_player2 ? 1 : 0].big_ammo_count--;
				game_panel->rad_shoot_type[player1_else_player2 ? 1 : 0]->SetString(1, "Big (3x3) : " + toString(game_panel->ammunition[player1_else_player2 ? 1 : 0].big_ammo_count) + " left");
			}
			else
			{
				wxMessageBox("There`s no such ammo left", "Out of ammo", wxOK | wxICON_INFORMATION);
				return;
			}
			break;
		}
		case 3:
		{
			if (game_panel->ammunition[player1_else_player2 ? 1 : 0].torpedo_count > 0)
			{
				game_panel->ammunition[player1_else_player2 ? 1 : 0].torpedo_count--;
				game_panel->rad_shoot_type[player1_else_player2 ? 1 : 0]->SetString(2, "Torpedo : " + toString(game_panel->ammunition[player1_else_player2 ? 1 : 0].torpedo_count) + " left");
			}
			else
			{
				wxMessageBox("There`s no such ammo left", "Out of ammo", wxOK | wxICON_INFORMATION);
				return;
			}
			break;
		}
		}*/
		while (true)
		{
			x = rand() % pre_begin_panel->GetMapSize();
			y = rand() % pre_begin_panel->GetMapSize();

			if (layout_panel->ship_map[0][x][y] >= 0)
				return;
		}
	}

	void bot_shoot_medium(int& x, int& y, vector<vector<int>> map)
	{
		game_panel->rad_shoot_type[1]->SetSelection(0);	//use regular ammo

		srand(time(NULL));

		std::pair<int, int> target;
		bool isMakeRandomMove = true;

		// Check if there are any previous hits
		for (int i = 0; i < pre_begin_panel->GetMapSize(); i++) 
		{
			for (int j = 0; j < pre_begin_panel->GetMapSize(); j++) 
			{
				if (map[i][j] == -2) {
					isMakeRandomMove = false;

					// Check neighboring positions
					std::vector<std::pair<int, int>> neighbors;
					if (i - 1 >= 0 && map[i - 1][j] >= 0) 
					{
						neighbors.push_back({ i - 1, j });
					}
					else if (i + 1 < pre_begin_panel->GetMapSize() && map[i + 1][j] >= 0) 
					{
						neighbors.push_back({ i + 1, j });
					}
					else if (j - 1 >= 0 && map[i][j - 1] >= 0) 
					{
						neighbors.push_back({ i, j - 1 });
					}
					else if (j + 1 < pre_begin_panel->GetMapSize() && map[i][j + 1] >= 0) 
					{
						neighbors.push_back({ i, j + 1 });
					}
					else 
						continue;

					// Randomly select a neighboring position as the target
					target = neighbors[rand() % neighbors.size()];
					x = target.first;
					y = target.second;

					return;
				}
			}
		}

		return bot_shoot_easy(x, y);
	}

	void bot_shoot_hard(int& x, int& y/*, vector<vector<int>> map*/)
	{
		game_panel->rad_shoot_type[1]->SetSelection(0);	//use regular ammo

		bot_shoot_easy(x, y);
	}

	//---------- Bot Difficulty ----------

	//---------- Shoot Types ----------

	bool regularShoot(const int x, const  int y, const int map_idx)
	{
		if (layout_panel->ship_map[map_idx][x][y] == 1)
		{
			cellHit(x, y, map_idx);
			return true;
		}
		else
		{
			cellMiss(x, y, map_idx);
			return false;
		}
	}

	bool bigShoot(const int x, const  int y, const int map_idx)
	{
		bool isHit = regularShoot(x, y, map_idx);

		// ---------- Check the surrounding cells ----------

		if(x < layout_panel->map_size - 1)
			layout_panel->ship_map[map_idx][x + 1][y] == 1 ? isHit = cellHit(x + 1, y, map_idx) : cellMiss(x + 1, y, map_idx);
		if(y < layout_panel->map_size - 1)
			layout_panel->ship_map[map_idx][x][y + 1] == 1		? isHit = cellHit(x, y + 1, map_idx) : cellMiss(x, y + 1, map_idx);
		if(x < layout_panel->map_size - 1 && y < layout_panel->map_size - 1)
			layout_panel->ship_map[map_idx][x + 1][y + 1] == 1	? isHit = cellHit(x + 1, y + 1, map_idx) : cellMiss(x + 1, y + 1, map_idx);
		if(x > 0)
			layout_panel->ship_map[map_idx][x - 1][y] == 1		? isHit = cellHit(x - 1, y, map_idx) : cellMiss(x - 1, y, map_idx);
		if(y > 0)
			layout_panel->ship_map[map_idx][x][y - 1] == 1		? isHit = cellHit(x, y - 1, map_idx) : cellMiss(x, y - 1, map_idx);
		if(x > 0 && y > 0)
			layout_panel->ship_map[map_idx][x - 1][y - 1] == 1	? isHit = cellHit(x - 1, y - 1, map_idx) : cellMiss(x - 1, y - 1, map_idx);
		if(x < layout_panel->map_size - 1 && y > 0)
			layout_panel->ship_map[map_idx][x + 1][y - 1] == 1	? isHit = cellHit(x + 1, y - 1, map_idx) : cellMiss(x + 1, y - 1, map_idx);
		if(x > 0 && y < layout_panel->map_size - 1)
			layout_panel->ship_map[map_idx][x - 1][y + 1] == 1	? isHit = cellHit(x - 1, y + 1, map_idx) : cellMiss(x - 1, y + 1, map_idx);

		// ---------- Check the surrounding cells ----------

		return isHit;
	}

	bool rowShoot(const int x, const  int y, const int map_idx)
	{
		bool isHit = regularShoot(x, 0, map_idx);

		for (int j = 1; j < layout_panel->ship_map[map_idx].size(); j++)
			layout_panel->ship_map[map_idx][x][j] == 1 ? isHit = cellHit(x, j, map_idx) : cellMiss(x, j, map_idx);

		return isHit;
	}

	//---------- Shoot Types ----------

	//---------- change cell state ----------

	bool cellHit(const int x, const  int y, const int map_idx)
	{
		game_panel->game_map[map_idx][x][y]->Disable();
		layout_panel->ship_map[map_idx][x][y] = -2;
		game_panel->game_map[map_idx][x][y]->SetLabelText("X");
		game_panel->game_map[map_idx][x][y]->SetBackgroundColour(wxColor(0xff, 0x4f, 0x00)); // orange color	

		return true;
	}

	bool cellMiss(const int x, const  int y, const int map_idx)
	{
		game_panel->game_map[map_idx][x][y]->Disable();
		layout_panel->ship_map[map_idx][x][y] = -1;
		game_panel->game_map[map_idx][x][y]->SetLabelText("");
		game_panel->game_map[map_idx][x][y]->SetBackgroundColour(wxColour(0x70, 0x80, 0x90)); //gray color

		return false;
	}

	//---------- change cell state ----------
};

class cApp : public wxApp
{
	Main_window* m_frame1 = nullptr;

public:
	virtual bool OnInit()
	{
		m_frame1 = new Main_window();
		m_frame1->Show();

		return true;
	}
};

wxIMPLEMENT_APP(cApp);

void Main_window::Close_start_panel_Open_pre_begin_panel(wxCommandEvent& evt)
{
	start_panel->Hide();
	pre_begin_panel->Show();

	Update_window();
}

void Main_window::Close_pre_begin_panel_Open_start_panel(wxCommandEvent& evt)
{
	pre_begin_panel->Hide();
	start_panel->Show();

	Update_window();
}

void Main_window::Close_pre_begin_panel_Open_layout_panel(wxCommandEvent& evt)
{
	if (pre_begin_panel->GetMapSize() == -1 || pre_begin_panel->GetDifficulty() == -1)
	{
		wxMessageBox("Choose an option!", "Error", wxOK | wxICON_INFORMATION);
		return;
	}

	layout_panel = new Layout_panel(this, pre_begin_panel->GetMapSize(), !bool(pre_begin_panel->GetDifficulty()));
	sizer_general->Add(layout_panel, 1, wxEXPAND);

	layout_panel->first_back_B->Bind(wxEVT_BUTTON, &Main_window::Close_layout_panel_Open_pre_begin_panel, this, layout_panel->first_back_B->GetId());


	if (! pre_begin_panel->GetDifficulty())// if two players
	{
		layout_panel->first_B->Bind(wxEVT_BUTTON, &Main_window::On_layout_panel_next, this, layout_panel->first_B->GetId());

		layout_panel->first_random_B->Bind(wxEVT_BUTTON, &Main_window::On_layout_panel_next, this, layout_panel->first_random_B->GetId());
		layout_panel->first_random_B->Bind(wxEVT_BUTTON, &Main_window::call_random_fill, this, layout_panel->first_random_B->GetId());

		layout_panel->second_back_B->Bind(wxEVT_BUTTON, &Main_window::On_layout_panel_prev, this, layout_panel->second_back_B->GetId());
		layout_panel->second_B->Bind(wxEVT_BUTTON, &Main_window::Close_layout_panel_Open_game_panel, this, layout_panel->second_B->GetId());

		layout_panel->second_random_B->Bind(wxEVT_BUTTON, &Main_window::Close_layout_panel_Open_game_panel, this, layout_panel->second_random_B->GetId());
	layout_panel->second_random_B->Bind(wxEVT_BUTTON, &Main_window::call_random_fill, this, layout_panel->second_random_B->GetId());
	}

	if (pre_begin_panel->GetDifficulty())// if one player
	{
		//random_fill_map();

		layout_panel->first_B->Bind(wxEVT_BUTTON, &Main_window::Close_layout_panel_Open_game_panel, this, layout_panel->first_B->GetId());

		layout_panel->first_random_B->Bind(wxEVT_BUTTON, &Main_window::Close_layout_panel_Open_game_panel, this, layout_panel->first_random_B->GetId());
		layout_panel->first_random_B->Bind(wxEVT_BUTTON, &Main_window::call_random_fill, this, layout_panel->first_random_B->GetId());
	}

	if (pre_begin_panel->GetMapSize() == 15)
		this->SetMinSize(wxSize(720, 600));
	else if (pre_begin_panel->GetMapSize() == 20)
		this->SetMinSize(wxSize(960, 800));

	pre_begin_panel->Hide();
	layout_panel->Show();

	Update_window();
}

void Main_window::Close_layout_panel_Open_pre_begin_panel(wxCommandEvent& evt)
{
	layout_panel->ship_map.clear();

	layout_panel->Hide();
	pre_begin_panel->Show();

	Update_window();
}

void Main_window::Close_layout_panel_Open_game_panel(wxCommandEvent& evt)
{
	game_panel = new Game_panel(this, pre_begin_panel->GetMapSize(), pre_begin_panel->GetDifficulty());

	//---------- Amunition ----------

	switch (pre_begin_panel->GetMapSize())
	{
	case 10:
	{
		game_panel->ammunition[0].big_ammo_count = 1;
		game_panel->ammunition[0].torpedo_count = 1;
		game_panel->ammunition[1].big_ammo_count = 1;
		game_panel->ammunition[1].torpedo_count = 1;

		game_panel->rad_shoot_type[0]->SetString(1, "Big(3x3) : 1 left");
		game_panel->rad_shoot_type[0]->SetString(2, "Torpedo : 1 left");
		game_panel->rad_shoot_type[1]->SetString(1, "Big(3x3) : 1 left");
		game_panel->rad_shoot_type[1]->SetString(2, "Torpedo : 1 left");

		break;
	}
	case 15:
	{
		game_panel->ammunition[0].big_ammo_count = 2;
		game_panel->ammunition[0].torpedo_count = 1;
		game_panel->ammunition[1].big_ammo_count = 2;
		game_panel->ammunition[1].torpedo_count = 1;

		game_panel->rad_shoot_type[0]->SetString(1, "Big(3x3) : 2 left");
		game_panel->rad_shoot_type[0]->SetString(2, "Torpedo : 1 left");
		game_panel->rad_shoot_type[1]->SetString(1, "Big(3x3) : 2 left");
		game_panel->rad_shoot_type[1]->SetString(2, "Torpedo : 1 left");
		break;
	}
	case 20:
	{
		game_panel->ammunition[0].big_ammo_count = 3;
		game_panel->ammunition[0].torpedo_count = 2;
		game_panel->ammunition[1].big_ammo_count = 3;
		game_panel->ammunition[1].torpedo_count = 2;

		game_panel->rad_shoot_type[0]->SetString(1, "Big(3x3) : 3 left");
		game_panel->rad_shoot_type[0]->SetString(2, "Torpedo : 2 left");
		game_panel->rad_shoot_type[1]->SetString(1, "Big(3x3) : 3 left");
		game_panel->rad_shoot_type[1]->SetString(2, "Torpedo : 2 left");
		break;
	}
	}

	//---------- Amunition ----------

	for (int i = 0; i < game_panel->game_map[0].size(); i++)
	{
		for (int j = 0; j < game_panel->game_map[0][i].size(); j++)
		{
			game_panel->game_map[0][i][j]->Bind(wxEVT_BUTTON, &Main_window::make_move, this, game_panel->game_map[0][i][j]->GetId());
			game_panel->game_map[1][i][j]->Bind(wxEVT_BUTTON, &Main_window::make_move, this, game_panel->game_map[1][i][j]->GetId());
		}
	}

	disableButtons();

	layout_panel->Hide();
	sizer_general->Add(game_panel, 1, wxEXPAND);

	auto g = layout_panel->ship_map;

	this->SetMinSize(wxSize(1680, 960));
	SetPosition(wxPoint(0, 0));
	//this->Centre();

	game_panel->pause_B->Bind(wxEVT_BUTTON, &Main_window::Open_pause_panel, this, game_panel->pause_B->GetId());


	Update_window();
}

void Main_window::Close_game_panel_Open_start_panel(wxCommandEvent& evt)
{
	layout_panel->ship_map.clear();
	player1_else_player2 = true;
	botMove = false;

	pause_window->Hide();
	game_panel->Hide();
	start_panel->Show();
	Update_window();
}

void Main_window::Open_pause_panel(wxCommandEvent& evt)
{
	pause_window->SetPosition(GetWindowCenter(this, pause_window));
	pause_window->Show();
}

void Main_window::Open_options_panel(wxCommandEvent& evt)
{
	options_window->old_isFullscreen = options_window->isFullscreen->GetValue();
	options_window->old_isInstantMove = options_window->isInstantMove->GetValue();

	pause_window->Hide();
	options_window->SetPosition(GetWindowCenter(this, options_window));
	options_window->Show();
}

void Main_window::On_layout_panel_next(wxCommandEvent& evt)
{
	layout_panel->first_player->Hide();
	layout_panel->second_player->Show();

	Update_window();
}

void Main_window::On_layout_panel_prev(wxCommandEvent& evt)
{
	layout_panel->ship_map.clear();

	layout_panel->second_player->Hide();
	layout_panel->first_player->Show();

	Update_window();
}
