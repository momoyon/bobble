#ifdef __linux__
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#endif // __linux__

#include "raylib.h"
#include "asset_packer.h"
#define COMMONLIB_REMOVE_PREFIX
#define COMMONLIB_IMPLEMENTATION
#include <commonlib.h>
#define ENGINE_IMPLEMENTATION
#include <engine.h>

#ifndef PACK_FOLDER
#define _PACK_FOLDER "include/packed"
#define PACK_FOLDER _PACK_FOLDER"/"
#endif // PACK_FOLDER

#ifndef ASSET_FOLDER
#define ASSET_FOLDER "resources/"
#endif // ASSET_FOLDER

bool pack_assets(const char *pack_folder_path, const char *assets_folder_path) {
	if (!DirectoryExists(pack_folder_path)) {
		log_info("Creating pack folder: %s", pack_folder_path);
		if (MakeDirectory(pack_folder_path) != 0) {
			log_error("Failed to create pack folder: %s", pack_folder_path);
			return false;
		}
	}

	FilePathList gfx_list = LoadDirectoryFiles(TextFormat("%s/gfx", assets_folder_path));

	for (int i = 0; i < gfx_list.count; ++i) {
		const char* filepath = gfx_list.paths[i];
		if (IsFileExtension(filepath, ".png")) {
			log_info("Packing PNG: %s", filepath);
			int data_size = -1;
			unsigned char* data = LoadFileData(filepath, &data_size);

			const char* basename = GetFileName(filepath);
			const char* packed_filepath = TextFormat("%s%s.h", pack_folder_path, basename);

			if (data_size == -1) {
				log_error("Failed to read file: %s", filepath);
				return false;
			}

			if (!ExportDataAsCode(data, data_size, packed_filepath)) {
				log_error("Failed to export data as code: %s to %s", filepath, packed_filepath);
				return false;
			}

			UnloadFileData(data);

			log_info("		- Data size: %d bytes", data_size);
		}
	}

  return true;
}

/// TODO: Make os_remove_dir() in commonlib
#ifdef  __linux__
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
  (void)sb; (void)typeflag; (void)ftwbuf;
  return remove(fpath);
}

bool rmrf(const char *path) {
  return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS) == 0;
}
#else
bool rmrf(const char *path) {
  ASSERT(false, "Windows version of rmrf() is not implemeted yet!");
  return false;
}

#endif /* ifdef  __linux__ */

int main(void) {

  if (DirectoryExists(_PACK_FOLDER".old")) {
    ASSERT(rmrf(_PACK_FOLDER".old"), "Deleting the folder should be successfull if it exists(most of the time)...");
  }

  if (DirectoryExists(PACK_FOLDER)) {
    if (rename(_PACK_FOLDER, _PACK_FOLDER".old") != 0) {
      log_error("Failed to rename %s -> %s.old", _PACK_FOLDER, _PACK_FOLDER);
    }
  }

  pack_assets(PACK_FOLDER, ASSET_FOLDER);

  return 0;
}
