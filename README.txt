# Exterra
FPS Game In Unreal Engine 4 (Group project for CS426)

Instructions: 3 options
	Open .sln file in visual studio then run it
	or open project in unreal's launcher
	Or just unzip the standalone and launch the .exe to play the game without requiring UE4
	The standalone releases can be found here: https://github.com/Team-Honey-Badger/Exterra/releases

Team Overview:
	Lead AI developer: Alexander Barsukov
		Developed:
			Boss fights
			NPC behaviour and systems (spawners and waypoints)
			Player movement controls
			Respawning and level progression
	Lead level developer: Joshua Knight
		Developed:
			Level layouts
			Doors and moving platforms
			Health packs and keys
			Main menu
	Lead weapon developer: George Bencivengo
		Developed:
			Weapons
			Weapon inventory
			Ammo system 

Controls:
	WASD for movement
	Left Shift to Sprint (press while walking, not before)
	Space to jump
	R to reload (no animation, instant reload)
	Left Mouse Button to shoot
	Escape to exit game
	F10 to suicide
	Number keys for weapon selection
	Mouse wheel for switching weapons (must select one first)
	
Objectives:
	Main:
		Defeat the boss to progress to the next level
	Minor:
		Locate keys in green doors to unlock red doors
		
Boss info:
	Found in BossInfo.txt file
	Warning: Contains spoilers
			
Known Bugs:
	AI:
		Bosses not well explained in-game
		Boss 1 can shoot itself
		NavMesh related bugs
	Levels:
		Visual bug when looking up (at least in large rooms of level 3)
		Some holes under doorways (fixed one but there might be more)
		Visibility is limited (feature?)
		Jumping on upwards lift can lead to falling through it
		NPCs might be able to trigger some lifts, not entirely sure
		VERY hard to notice that you picked up the left side's key in level 1 (recommend sound affect or HUD indication)
		Teleporters don't work as intended (disabled atm)
	Teleporters:
		Utilized atm
		Pulls you back in after exiting (no escape!)	
	Weapons:
		[SERIOUS] Weapon selection has empty slots even after picking up all the guns, causing the game to crash!!!!!!
			Steps to reproduce:
				Go to level 2 (from menu or just load level 2)
				Walk about 7 meters forward
				Press 5 to pull out Rocket Launcher
				Scroll mouse wheel up
				There will be weapon gaps
			Notes:
				I think this might relate to gun pickup order
				What could walking have to do with it? Time based issue?
				I think one quick fix would be to make the player pull out a gun as soon as they grab it
		Cannot fire through trigger volumes and other invisible objects:
			Some areas in levels feel awkward (NPCs removed from those areas)
			Current solution is to place trigger volumes on the ground where they won't be in the way
		Fire rates are to fast (delta time not used)
		Damage is too high (1 hit kills)
		Sound players after death
	Sounds:
		Not enough sounds
		Some sounds not playing at correct locations
	Misc:
		No proper win condition (that's rather big)
		No HUD indications of fatigue and timers (respawn/level progression)
		No HUD for keys remaining
		Death after defeating a boss will likely let you progress anyway  
		There are unused assets that take up space
		