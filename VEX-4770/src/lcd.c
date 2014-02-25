/*
 * lcd.c
 *
 *  Created on: Jan 30, 2014
 *      Author: Jacob
 */


#include "main.h"
#include <API.h>
#include "motors.h"

#define vexCompetitionState ( (isEnabled()?0:2) + isAutonomous()?1:0 )

#define LCD_BTN_NONE 0

int getLcdButtons() {
	int competitionState = vexCompetitionState;
	int buttons;

	while (lcdReadButtons(uart1) != LCD_BTN_NONE) {
		if (vexCompetitionState != competitionState)
			return LCD_BTN_NONE;
		taskDelay(10);
	}

	do {
		buttons = lcdReadButtons(uart1);

		if (vexCompetitionState != competitionState)
			return LCD_BTN_NONE;

		taskDelay(10);
	} while (buttons == LCD_BTN_NONE);

	return buttons;
}

static int autonSelection = -1;
int button;

void autonSelectMenu() {
	lcdSetBacklight(uart1, true);
	while (button != LCD_BTN_CENTER) {
		lcdClear(uart1);

		lcdSetText(uart1, 1, "Alliance Color?");
		lcdSetText(uart1, 2, "< Red     Blue >");

		button = getLcdButtons();

		if (button == LCD_BTN_LEFT) {
			lcdClear(uart1);

			lcdSetText(uart1, 1, "Starting Side?");
			lcdSetText(uart1, 2, "< Middle  Hang >");

			while (button != LCD_BTN_LEFT || button != LCD_BTN_RIGHT) {
				button = getLcdButtons();

				// Red Middle
				if (button == LCD_BTN_LEFT) {
					int screen = 0;
					int minScreen = 0;
					int maxScreen = 4;

					lcdClear(uart1);

					while (button != LCD_BTN_CENTER) {
						switch (screen) {
							case 0:
								lcdSetText(uart1, 1, "auton one");
								break;
							case 1:
								lcdSetText(uart1, 1, "auton two");
								break;
							case 2:
								lcdSetText(uart1, 1, "auton three");
								break;
							case 3:
								lcdSetText(uart1, 1, "auton four");
								break;
						}

						button = getLcdButtons();

						if (button == LCD_BTN_RIGHT) {
							if (++screen > maxScreen)
								screen = minScreen;
						} else if (button == LCD_BTN_LEFT) {
							if (--screen < minScreen)
								screen = maxScreen;
						}
					}

					autonSelection = screen;
					lcdSetText(uart1, 1, "   AUTONOMOUS");
					lcdSetText(uart1, 2, "      SET");
					taskDelay(400);
					return;
				}

				// Red Hang
				if (button == LCD_BTN_RIGHT) {
					int screen = 0;
					int minScreen = 0;
					int maxScreen = 4;

					lcdClear(uart1);

					while (button != LCD_BTN_CENTER) {
						switch (screen) {
							case 0:
								lcdSetText(uart1, 1, "auton one");
								break;
							case 1:
								lcdSetText(uart1, 1, "auton two");
								break;
							case 2:
								lcdSetText(uart1, 1, "auton three");
								break;
							case 3:
								lcdSetText(uart1, 1, "auton four");
								break;
						}

						button = getLcdButtons();

						if (button == LCD_BTN_RIGHT) {
							if (++screen > maxScreen)
							screen = minScreen;
						} else if (button == LCD_BTN_LEFT) {
							if (--screen < minScreen)
								screen = maxScreen;
						}
					}

					autonSelection = screen;
					lcdSetText(uart1, 1, "   AUTONOMOUS");
					lcdSetText(uart1, 2, "      SET");
					taskDelay(400);
					return;
				}
			}
		}

		if (button == LCD_BTN_RIGHT) {
			lcdClear(uart1);

			lcdSetText(uart1, 1, "Starting Side?");
			lcdSetText(uart1, 2, "< Middle  Hang >");

			while (button != LCD_BTN_LEFT || button != LCD_BTN_RIGHT) {
				button = getLcdButtons();

				// Blue Middle
				if (button == LCD_BTN_LEFT) {
					int screen = 0;
					int minScreen = 0;
					int maxScreen = 4;

					lcdClear(uart1);

					while (button != LCD_BTN_CENTER) {
						switch (screen) {
							case 0:
								lcdSetText(uart1, 1, "auton one");
								break;
							case 1:
								lcdSetText(uart1, 1, "auton two");
								break;
							case 2:
								lcdSetText(uart1, 1, "auton three");
								break;
							case 3:
								lcdSetText(uart1, 1, "auton four");
								break;
						}

						button = getLcdButtons();

						if (button == LCD_BTN_RIGHT) {
							if (++screen > maxScreen)
								screen = minScreen;
						} else if (button == LCD_BTN_LEFT) {
							if (--screen < minScreen)
								screen = maxScreen;
						}
					}

					autonSelection = screen;
					lcdSetText(uart1, 1, "   AUTONOMOUS");
				    lcdSetText(uart1, 2, "      SET");
					taskDelay(400);
					return;
				}

				// Blue Hang
				if (button == LCD_BTN_RIGHT) {
					int screen = 0;
					int minScreen = 0;
					int maxScreen = 4;

					lcdClear(uart1);

					while (button != LCD_BTN_CENTER) {
						switch (screen) {
							case 0:
								lcdSetText(uart1, 1, "auton one");
								break;
							case 1:
								lcdSetText(uart1, 1, "auton two");
								break;
							case 2:
								lcdSetText(uart1, 1, "auton three");
								break;
							case 3:
								lcdSetText(uart1, 1, "auton four");
								break;
						}

						button = getLcdButtons();

						if (button == LCD_BTN_RIGHT) {
							if (++screen > maxScreen)
								screen = minScreen;
						} else if (button == LCD_BTN_LEFT) {
							if (--screen < minScreen)
								screen = maxScreen;
						}
					}

					autonSelection = screen;
					lcdSetText(uart1, 1, "   AUTONOMOUS");
					lcdSetText(uart1, 2, "      SET");
					taskDelay(400);
					return;
				}
			}
		}
	}
}

void robotStatusMenu() {
	int screen = 0;
	int maxScreen = 6;
	int minScreen = 0;

	lcdSetBacklight(uart1, true);
	while (button != LCD_BTN_CENTER) {
		switch (screen) {
			case 0:
				lcdSetText(uart1, 1, "  Main Battery");
				lcdPrint(uart1, 2, "%d mV", powerLevelMain());
				break;
			case 1:
				lcdSetText(uart1, 1, " Backup Battery");
				lcdPrint(uart1, 2, "%d mV", powerLevelBackup());
				break;
			case 2:
				lcdSetText(uart1, 1, "    Joystick");
				lcdSetText(uart1, 2, isJoystickConnected(1) ? "    Connected" : "  Not Connected");
				break;
			case 3:
				lcdSetText(uart1, 1, "Field Controller");
				lcdSetText(uart1, 2, isOnline() ? "    Connected" : "  Not Connected");
				break;
			case 4:
				lcdSetText(uart1, 1, " IMEs Connected");
				lcdSetText(uart1, 2, "xxxxxxxxxxxxxxxx");
				break;
			case 5:
				lcdPrint(uart1, 1, "Left Pot %d", analogRead(potLiftLeft));
				lcdPrint(uart1, 2, "Left Mtr %d", motorGet(liftLeft));
				break;
			case 6:
				lcdPrint(uart1, 1, "Right Pot %d", analogRead(potLiftRight));
				lcdPrint(uart1, 2, "Right Mtr %d", motorGet(liftRight));
				break;
		}

		button = getLcdButtons();
		if (button == LCD_BTN_RIGHT) {
			if (++screen > maxScreen)
				screen = minScreen;
		} else if (button == LCD_BTN_LEFT) {
			if (--screen < minScreen)
				screen = maxScreen;
		}

		taskDelay(10);
	}
}

void lcdMenu() {
	lcdInit(uart1);
	lcdClear(uart1);
	lcdSetBacklight(uart1, true);

	while (true) {
		if (!isEnabled())
			lcdSetBacklight(uart1, false);

		lcdClear(uart1);

		lcdSetText(uart1, 1, "< Auton Select");
		lcdSetText(uart1, 2, "  Robot Status >");

		button = getLcdButtons();

		if (button == LCD_BTN_LEFT) {
			autonSelectMenu();
		} else if (button == LCD_BTN_RIGHT) {
			robotStatusMenu();
		}

		taskDelay(10);
	}
}
