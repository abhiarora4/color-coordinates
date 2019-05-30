#include "color.h"

#include <iostream>
#include <algorithm>
#include <string>

#include <stdio.h>
#include <math.h>

Color::Color(void)
{

}

Color::Color(const std::string& path)
{
	auto getDirEntry = [](std::string path, std::string dir) {
		std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);
		for (auto& e: std::filesystem::recursive_directory_iterator(path)) {
			// std::cout << "Directory: " << e << " My: " << path << " D: " << dir << std::endl;
			if (!e.is_directory())
				continue;
			if (!e.path().stem().compare(dir))
				return e;
		}
		// std::cout << "Kuch ni hua" << std::endl;
		return std::filesystem::directory_entry();
	};

	auto cmfsDirEntry = getDirEntry(path, "cmfs");
	auto illuminantsDirEntry = getDirEntry(path, "illuminants");

	for (auto& e: std::filesystem::directory_iterator(illuminantsDirEntry.path())) {
		if (e.is_directory())
			continue;
		if (e.path().extension().compare(".csv"))
			continue;

		XYData data;
		data.fromFile(e.path());
		std::string filename = e.path().stem();
		std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
		illuminants_.insert(std::make_pair(filename, data));
	}

	for (auto& e: std::filesystem::directory_iterator(cmfsDirEntry.path())) {
		std::cout << "E " << e << std::endl;
		if (!e.is_directory())
			continue;

		std::string dirName = e.path().stem();
		std::transform(dirName.begin(), dirName.end(), dirName.begin(), ::tolower);
		ColorObserver obs(std::string(e.path()));
		observer_.insert(std::make_pair(dirName, obs));
	}
}

ColorObserver& Color::get_observer(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	return observer_[name];
}

bool Color::has_observer(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (observer_.find(name) == observer_.end())
		return false;
	return true;
}

XYData& Color::get_illuminant(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	return illuminants_[name];
}

bool Color::has_illuminant(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	if (illuminants_.find(name) == illuminants_.end())
		return false;
	return true;
}

Color::TriValues Color::sd_to_XYZ(XYData sd, std::string illuminant, std::string observer)
{
	TriValues value = {0,0,0};
	long double n = 0;
    const double diff = 1;

    ColorObserver obs;
    XYData src;
    
	if (!has_illuminant(illuminant))
    	return value;

    if (!has_observer(observer))
    	return value;

    src = get_illuminant(illuminant);
    obs = get_observer(observer);

    for (std::size_t i = 0; i < sd.length(); i++)
        n += obs.get_y().y_at_ndx(i) * src.y_at_ndx(i) * diff;

    for (std::size_t i = 0; i < sd.length(); i++) {
        const double c = sd.y_at_ndx(i) * src.y_at_ndx(i) * diff;

        value.x += c * obs.get_x().y_at_ndx(i);
        value.y += c * obs.get_y().y_at_ndx(i);
        value.z += c * obs.get_z().y_at_ndx(i);
    }

    value.x /= n;
    value.y /= n;
    value.z /= n;

    return value;
}


Color::TriValues Color::sd_to_lab(XYData sd, std::string illuminant, std::string observer)
{
	TriValues value = {0,0,0};
	return value;
}

Color::TriValues Color::ref_XYZ(std::string illuminant, std::string observer)
{
	TriValues value = {0,0,0};
	return value;
}


