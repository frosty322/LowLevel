#pragma once

#include <structs.h>

struct Image* Load(const char* filename);

enum write_status Save(const char* filename, struct Image const * image);
