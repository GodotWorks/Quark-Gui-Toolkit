/*************************************************************************/
/*  register_core_types.cpp                                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "register_core_types.h"

#include "bind/core_bind.h"
#include "class_db.h"
#include "core_string_names.h"
#include "engine.h"
#include "func_ref.h"
#include "geometry.h"
#include "input_map.h"
#include "io/config_file.h"
#include "io/marshalls.h"
#include "io/resource_import.h"
#include "math/a_star.h"
#include "math/triangle_mesh.h"
#include "os/input.h"
#include "os/main_loop.h"
#include "packed_data_container.h"
#include "path_remap.h"
#include "project_settings.h"
#include "undo_redo.h"
static ResourceFormatImporter *resource_format_importer = NULL;

static _ResourceLoader *_resource_loader = NULL;
static _ResourceSaver *_resource_saver = NULL;
static _OS *_os = NULL;
static _Engine *_engine = NULL;
static _ClassDB *_classdb = NULL;
static _Marshalls *_marshalls = NULL;

static _Geometry *_geometry = NULL;

extern Mutex *_global_mutex;

extern void register_global_constants();
extern void unregister_global_constants();
extern void register_variant_methods();
extern void unregister_variant_methods();

void register_core_types() {

	ObjectDB::setup();
	ResourceCache::setup();
	MemoryPool::setup();

	_global_mutex = Mutex::create();

	StringName::setup();

	register_global_constants();
	register_variant_methods();

	CoreStringNames::create();

	resource_format_importer = memnew(ResourceFormatImporter);
	ResourceLoader::add_resource_format_loader(resource_format_importer);

	ClassDB::register_class<Object>();

	ClassDB::register_virtual_class<Script>();

	ClassDB::register_class<Reference>();
	ClassDB::register_class<WeakRef>();
	ClassDB::register_class<Resource>();
	ClassDB::register_class<Image>();

	ClassDB::register_virtual_class<InputEvent>();
	ClassDB::register_virtual_class<InputEventWithModifiers>();
	ClassDB::register_class<InputEventKey>();
	ClassDB::register_virtual_class<InputEventMouse>();
	ClassDB::register_class<InputEventMouseButton>();
	ClassDB::register_class<InputEventMouseMotion>();
	ClassDB::register_class<InputEventJoypadButton>();
	ClassDB::register_class<InputEventJoypadMotion>();
	ClassDB::register_class<InputEventScreenDrag>();
	ClassDB::register_class<InputEventScreenTouch>();
	ClassDB::register_class<InputEventAction>();
	ClassDB::register_virtual_class<InputEventGesture>();
	ClassDB::register_class<InputEventMagnifyGesture>();
	ClassDB::register_class<InputEventPanGesture>();

	ClassDB::register_class<FuncRef>();
	ClassDB::register_class<MainLoop>();
	//ClassDB::register_type<OptimizedSaver>();
	ClassDB::register_class<UndoRedo>();
	ClassDB::register_class<TriangleMesh>();

	ClassDB::register_virtual_class<ResourceInteractiveLoader>();

	ClassDB::register_class<_File>();
	ClassDB::register_class<_Directory>();
	ClassDB::register_class<_Thread>();
	ClassDB::register_class<_Mutex>();
	ClassDB::register_class<_Semaphore>();

	ClassDB::register_class<ConfigFile>();

	ClassDB::register_class<PackedDataContainer>();
	ClassDB::register_virtual_class<PackedDataContainerRef>();
	ClassDB::register_class<AStar>();
	ClassDB::register_class<EncodedObjectAsID>();

	_geometry = memnew(_Geometry);

	_resource_loader = memnew(_ResourceLoader);
	_resource_saver = memnew(_ResourceSaver);
	_os = memnew(_OS);
	_engine = memnew(_Engine);
	_classdb = memnew(_ClassDB);
	_marshalls = memnew(_Marshalls);
}

void register_core_settings() {
}

void register_core_singletons() {

	ClassDB::register_class<ProjectSettings>();
	ClassDB::register_class<_Geometry>();
	ClassDB::register_class<_ResourceLoader>();
	ClassDB::register_class<_ResourceSaver>();
	ClassDB::register_class<_OS>();
	ClassDB::register_class<_Engine>();
	ClassDB::register_class<_ClassDB>();
	ClassDB::register_class<_Marshalls>();
	ClassDB::register_virtual_class<Input>();
	ClassDB::register_class<InputMap>();

	Engine::get_singleton()->add_singleton(Engine::Singleton("ProjectSettings", ProjectSettings::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Geometry", _Geometry::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceLoader", _ResourceLoader::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceSaver", _ResourceSaver::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("OS", _OS::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Engine", _Engine::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ClassDB", _classdb));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Marshalls", _Marshalls::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Input", Input::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("InputMap", InputMap::get_singleton()));
}

void unregister_core_types() {

	memdelete(_resource_loader);
	memdelete(_resource_saver);
	memdelete(_os);
	memdelete(_engine);
	memdelete(_classdb);
	memdelete(_marshalls);

	memdelete(_geometry);

	if (resource_format_importer)
		memdelete(resource_format_importer);

	ObjectDB::cleanup();

	unregister_variant_methods();
	unregister_global_constants();

	ClassDB::cleanup();
	ResourceCache::clear();
	CoreStringNames::free();
	StringName::cleanup();

	if (_global_mutex) {
		memdelete(_global_mutex);
		_global_mutex = NULL; //still needed at a few places
	};

	MemoryPool::cleanup();
}
