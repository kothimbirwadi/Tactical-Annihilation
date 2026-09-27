# UnitView.gd
# Godot 4 GDScript for Member 3: The Combat Specialist (Frontend Vertical Slice)
# Attaches to the reusable 3D Unit Scene.
extends Node3D
class_name UnitView

@export var unit_name: String = "Operative"
@export var max_health: int = 100
var current_health: int = 100

# Nodes
@onready var model_pivot: Node3D = $ModelPivot # Blender .glb model attached here
@onready var health_bar_label: Label3D = $HealthBar3D
@onready var animation_player: AnimationPlayer = $AnimationPlayer

func _ready() -> void:
	current_health = max_health
	update_health_bar()

# -------------------------------------------------------------
# 1. 3D Floating Health Bar
# -------------------------------------------------------------
func set_health(new_health: int) -> void:
	current_health = clamp(new_health, 0, max_health)
	update_health_bar()

func update_health_bar() -> void:
	if health_bar_label:
		health_bar_label.text = "%d / %d HP" % [current_health, max_health]
		if current_health <= 0:
			health_bar_label.modulate = Color.RED
			health_bar_label.text = "KIA"
		elif current_health < (max_health * 0.3):
			health_bar_label.modulate = Color.ORANGE
		else:
			health_bar_label.modulate = Color.GREEN

# -------------------------------------------------------------
# 2. Visual Effects (VFX) & Attack Animations
# -------------------------------------------------------------

## Standard 2-AP Attack Visual Effect
func play_standard_attack_vfx(target_position: Vector3) -> void:
	print("[%s] Playing standard attack VFX towards %s" % [unit_name, target_position])
	# Look towards the target
	look_at(Vector3(target_position.x, global_position.y, target_position.z), Vector3.UP)
	
	# Quick attack recoil or animation
	var tween = create_tween()
	var forward_step = -transform.basis.z * 0.3
	tween.tween_property(self, "position", position + forward_step, 0.1)
	tween.tween_property(self, "position", position, 0.15)

## Sniper Ultimate: Piercing Shot VFX (Line beam through grid tiles)
func play_piercing_shot_vfx(beam_end_position: Vector3) -> void:
	print("[%s] ULTIMATE: Firing piercing beam to %s" % [unit_name, beam_end_position])
	# Creates a high-velocity piercing tracer beam
	var line = ImmediateMesh.new()
	var mesh_instance = MeshInstance3D.new()
	mesh_instance.mesh = line
	get_parent().add_child(mesh_instance)
	
	# Draw glowing beam from sniper muzzle to end tile
	var mat = StandardMaterial3D.new()
	mat.albedo_color = Color(1.0, 0.2, 0.1, 1.0)
	mat.emission_enabled = true
	mat.emission = Color(1.0, 0.4, 0.1)
	mesh_instance.material_override = mat

	# Flash and fade beam
	var tween = create_tween()
	tween.tween_property(mesh_instance, "scale", Vector3(1.5, 1.5, 1.5), 0.1)
	tween.tween_property(mesh_instance, "scale", Vector3.ZERO, 0.25)
	tween.tween_callback(mesh_instance.queue_free)

## Cavalry Ultimate: Chain Blitz Dash VFX (Dash attack between chained targets)
func play_cavalry_dash_vfx(next_tile_position: Vector3, on_complete_callback: Callable = Callable()) -> void:
	print("[%s] ULTIMATE: Dashing to %s" % [unit_name, next_tile_position])
	var tween = create_tween().set_trans(Tween.TRANS_QUAD).set_ease(Tween.EASE_OUT)
	# Fast dash to target position
	tween.tween_property(self, "global_position", next_tile_position, 0.18)
	if on_complete_callback.is_valid():
		tween.tween_callback(on_complete_callback)
