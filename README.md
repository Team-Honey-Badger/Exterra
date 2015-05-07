# Exterra
FPS Game In Unreal Engine 4 (Group project for CS426)

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
	Left Shift for Sprinting (press while walking, not before)
	Space to jump
	R to reload (no animation, instant reload)
	Left Mouse Button to shoot
	Escape to exit game
	
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
		Collision volume issues:
			Large arches cannot be entered
		Some holes under doorways:
			Level 3 boss door is one example
		Visibility is limited
		Jumping on upwards lift can lead to falling through it
		NPCs might be able to trigger some lifts, not entirely sure
		VERY hard to notice that you picked up the left side's key in level 1
	Teleporters:
		Utilized atm
		Pulls you back in after exiting (no escape!)	
	Weapons:
		Cannot fire through trigger volumes and other invisible objects:
			Doors are troublesome
			Some areas in levels feel awkward
		Fire rates are to fast
		Assault rifle range is too short
		Damage is too high (1 hit kills)
		Ammo's mag count is wrong
		Sound players after death
	Sounds:
		Not enough sounds
		Some sounds not playing at correct locations
	Misc:
		No proper win condition
		No HUD indications of fatigue and timers (respawn/level progression)
		No HUD for keys remaining
		Death after defeating a boss will likely let you progress anyway  
		There are unused assets that take up space
		