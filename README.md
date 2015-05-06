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

Known Bugs:
	AI:
		Sometimes enemies may spam roars
		Bosses not well explained in-game
		Boss 1 is too slow moving and can shoot itself
		Ranged AI can shoot each other (not sure if bug)
		Ranged AI don't make footsteps
		NavMesh related bugs
		
	Levels:
		Collision volume issues:
			Large arches cannot be entered
		Some holes under doorways:
			Level 3 boss door is one example
		Visibility is limited
	Weapons:
		Cannot fire through trigger volumes and other invisible objects:
			Doors are troublesome
			Some areas in levels feel awkward
		Fire rates are to fast
		Assault rifle range is too short
		Damage is too high (1 hit kills)
	Sounds:
		Almost everything is broken
	Misc:
		No proper win condition
		No HUD indications of fatigue and timers (respawn/level progression)
		