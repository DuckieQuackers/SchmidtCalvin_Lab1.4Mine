#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE();

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Game window", wxPoint(30,30), wxSize(800,600)) {
	//Create array to store the grid of button and the spacer to place them on screen
	btn = new wxButton * [nFieldWidth * nFieldHieght];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHieght, 0, 0);

	//Create array to store mine locations
	nField = new int[nFieldWidth * nFieldHieght];

	//Create a font to make the text more readable
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	//Populate spacer with buttons
	for (int i = 0; i < nFieldWidth; i++) {
		for (int j = 0; j < nFieldHieght; j++) {
			btn[j * nFieldWidth + i] = new wxButton(this, 10000 + (j * nFieldWidth + i));
			btn[j * nFieldWidth + i]->SetFont(font);
			grid->Add(btn[j * nFieldWidth + i], 1, wxEXPAND | wxALL);

			btn[j * nFieldWidth + i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			nField[j * nFieldWidth + i] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain() {
	delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent& evt) {

	//Get the location of the button clicked
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldHieght;

	//If this is the first button clicked, generate mindfield
	if (bFirstClick) {
		int mines = 30;

		while (mines) {
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHieght;

			if (nField[ry * nFieldWidth + rx] == 0 && rx != x && ry != y) {
				nField[ry * nFieldWidth + rx] = -1;
				mines--;
			}
		}

		bFirstClick = false;
	}

	//Set button to false to show its been clicked
	btn[y * nFieldWidth + x]->Enable(false);

	//Check to see if player hit a mine
	if (nField[y * nFieldWidth + x] == -1) {
		//If they did hit a mine
		wxMessageBox("Boom! Game over!");

		//Reset minefield
		bFirstClick = true;
		for (int i = 0; i < nFieldWidth; i++) {
			for (int j = 0; j < nFieldHieght; j++) {
				nField[j * nFieldWidth + i] = 0;
				btn[j * nFieldWidth + i]->SetLabel("");
				btn[j * nFieldWidth + i]->Enable(true);
			}
		}
	}
	else {
		//If they didnt hit a mine, count the number of surrounding mines
		int neighborMines = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				//Make sure the check doesnt go out of bounds
				if (x + i + 1 >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHieght) {
					if (nField[(y + j) * nFieldWidth + (x + i)] == -1) {
						neighborMines++;
					}
				}
			}
		}

		if (neighborMines > 0) {
			btn[y * nFieldWidth + x]->SetLabel(std::to_string(neighborMines));
		}
	}

	evt.Skip();
}