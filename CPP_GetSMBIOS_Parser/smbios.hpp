#include <vector>
#include <cstdint>

#ifndef __SMBIOS_H__
#define __SMBIOS_H__

namespace smbios {

	namespace types {
		enum {
			bios_info = 0, // Required
			system_info = 1, // Required
			baseboard_info = 2,
			module_info = 2,
			system_enclosure = 3, // Required
			system_chassis = 3, // Required
			processor_info = 4, // Required
			memory_controller_info = 5, // Obsolete
			memory_module_info = 6, // Obsolete
			cache_info = 7, // Required
			port_connector_info = 8,
			system_slots = 9, // Required
			onboard_device_info = 10, // Obsolete
			oem_strings = 11,
			system_config_options = 12,
			language_info = 13,
			group_associations = 14,
			system_event_log = 15,
			memory_array = 16, // Required
			memory_device = 17, // Required
			memory_error_info_32bit = 18,
			memory_array_mapped_addr = 19, // Required
			memory_device_mapped_addr = 20,
			builtin_pointing_device = 21,
			portable_battery = 22,
			system_reset = 23,
			hardware_security = 24,
			system_power_controls = 25,
			voltage_probe = 26,
			cooling_device = 27,
			temperature_probe = 28,
			electrical_current_probe = 29,
			out_of_band_remote_access = 30,
			bis_entry_point = 31, // Required
			system_boot_info = 32, // Required
			memory_error_info_64bit = 33,
			management_device = 34,
			management_device_component = 35,
			management_device_threshold = 36,
			memory_channel = 37,
			ipmi_device_info = 38,
			system_power_supply = 39,
			additional_info = 40,
			onboard_device_extinfo = 41,
			management_controller_host = 42,
			inactive = 126,
			end_of_table = 127, // Always last structure
		};
	}

	typedef uint8_t   byte_t;
	typedef uint16_t  word_t;
	typedef uint32_t dword_t;

#ifdef _MSC_VER
	typedef __int64 qword_t;
#else
#ifdef INT64_C
	typedef uint64_t qword_t;
#else
	typedef (unsigned long long int) qwordt_t;
#endif
#endif

	typedef byte_t str_id;
	typedef byte_t enum_t;

	typedef std::vector<char*> string_array_t;

	struct header;

	class parser {
	protected:
		byte_t* raw_data;
		size_t  raw_size;

	public:
		std::vector<header*> headers;

		parser() :raw_data(nullptr) {};
		parser(const parser& x) { feed(x.raw_data, x.raw_size); };
		virtual ~parser() { clear(); };

		static byte_t* skip(byte_t*);
		static header* extract_strings(header*, string_array_t&);

		void feed(const void* raw_smbios, size_t size);
		void clear();
	};

#pragma pack(push, 1)

	struct header {
		byte_t type;
		byte_t length;
		word_t handle;
	};

	struct string_list :public header {
		byte_t count;
	};

	struct baseboard_info :public header {
		byte_t ManufacturerName;
		byte_t ProductName;
		byte_t Version;
		byte_t SerialNumber;
		byte_t Product;
		byte_t Version1;
		byte_t SerialNumber1;
	};

	struct bios_info :public header {
		// 2.0
		str_id  vendor;
		str_id  version;
		word_t  starting_segment;
		str_id  release_date;
		byte_t  rom_size;
		qword_t characteristics;
		// 2.4
		byte_t ext_char1;
		byte_t ext_char2;
		byte_t sb_major;
		byte_t sb_minor;
		byte_t ec_major;
		byte_t ec_minor;
	};

	struct system_info :public header {
		// 2.0
		str_id manufacturer;
		str_id product_name;
		str_id version;
		str_id serial_number;
		// 2.1
		struct {
			dword_t time_low;
			word_t  time_mid;
			word_t  time_hi_and_version;
			byte_t  clock_seq_hi_and_reserved;
			byte_t  clock_seq_low;
			byte_t  node[6];
		} uuid;
		enum_t wakeup_type;
		// 2.4
		str_id sku;
		str_id family;
	};

	struct system_chassis :public header {
		// 2.0
		str_id  manufacturer;
		byte_t  type;
		str_id  version;
		str_id  serial_number;
		str_id  assert_tag;
		// 2.1
		enum_t  bootup_state;
		enum_t  power_supply_state;
		enum_t  thermal_state;
		enum_t  security_status;
		// 2.3
		dword_t oem;
		byte_t  height;
		byte_t  cords;
	};

	struct proc_info :public header {
		// 2.0
		str_id  socket_designation;
		enum_t  type;
		enum_t  family;
		str_id  manufacturer;
		qword_t id;
		str_id  version;
		byte_t  voltage;
		word_t  clock;
		word_t  speed_max;
		word_t  speed_cur;
		byte_t  status;
		enum_t  upgrade;
		// 2.1
		word_t  L1;
		word_t  L2;
		word_t  L3;
		// 2.3
		str_id  serial_number;
		str_id  assert_tag;
		str_id  part_number;
		// 2.5
		byte_t  cores;
		byte_t  cores_enabled;
		byte_t  threads;
		word_t  characteristics;
		enum_t  family2;
	};

	struct cache_info :public header {
		// 2.0
		str_id socket_designation;
		word_t config;
		word_t size_max;
		word_t size_cur;
		word_t sram_supported;
		word_t sram_cur;
		// 2.1
		byte_t speed;
		enum_t error_correction_type;
		enum_t system_cache_type;
		enum_t associativity;
	};

	struct slot :public header {
		// 2.0
		str_id slot_designation;
		enum_t type;
		enum_t data_bus_width;
		enum_t current_usage;
		enum_t length;
		word_t id;
		byte_t characteristics;
		// 2.1
		byte_t characteristics2;
		// 2.6
		word_t segment_group;
		byte_t bus;
		byte_t device;
	};

	typedef string_list oem_strings;
	typedef string_list system_config_options;

	struct lang_info :public header {
		byte_t installed_langs;
		byte_t flags;
		byte_t reserved[15];
		str_id current_lang;
	};

	struct mem_arr :public header {
		// 2.1
		enum_t  location;
		enum_t  use;
		enum_t  error_correction;
		dword_t capacity;
		word_t  error_info_handle;
		word_t  devices_number;
		// 2.7
		qword_t capacity_ext;
	};

	struct mem_device :public header {
		// 2.1
		word_t  mem_arr_handle;
		word_t  mem_arr_error_info_handle;
		word_t  total_width;
		word_t  data_width;
		word_t  size;
		enum_t  form_factor;
		byte_t  device_set;
		str_id  device_locator;
		str_id  bank_locator;
		enum_t  type;
		word_t  type_detail;
		// 2.3
		word_t  speed;
		str_id  manufacturer;
		str_id  serial_number;
		str_id  assert_tag;
		str_id  part_number;
		// 2.6
		byte_t  attributes;
		// 2.7
		dword_t size_ext;
		word_t  clock_speed;
		// 2.8
		word_t  voltage_min;
		word_t  voltage_max;
		word_t  voltage;
	};

#pragma pack(pop)

	// Implementation

	byte_t* parser::skip(byte_t* x) {
		byte_t* ptr = x + ((header*)x)->length;
		size_t len = 0;

		if (*ptr == 0) ptr += 2;
		else do {
			len = strlen((const char*)ptr);
			ptr += len + 1;
		} while (len > 0);

		return ptr;
	};

	header* parser::extract_strings(header* x, string_array_t& a) {
		byte_t* ptr = ((byte_t*)x) + x->length;

		a.clear();
		a.push_back(nullptr);

		if (*ptr == 0) ptr += 2;
		else for (;;) {
			char* str = (char*)ptr;
			size_t len = strlen(str);

			ptr += len + 1;

			if (len == 0) break;
			else a.push_back(str);
		}

		return (header*)ptr;
	}

	void parser::feed(const void* raw_smbios, size_t size) {
		clear();

		raw_size = size;
		raw_data = new byte_t[raw_size];

		memcpy(raw_data, raw_smbios, size);

		byte_t* x = raw_data;

		while ((size_t)(x - raw_data) < raw_size) {
			headers.push_back((header*)x);
			x = skip(x);
		}
	};

	void parser::clear() {
		headers.clear();
		delete[] raw_data;
		raw_size = 0;
	};

} // namespace smbios
#endif

/* EXAMPLE *//*
	using namespace smbios;
	parser meta;
	byte_t *buff = nullptr;
	size_t  buff_size = 0;
	//////////////////////////////////////////////////////
	// Code that makes buffer to point (or hold) smbios //
	// structure table. Don't forget about size.        //
	//////////////////////////////////////////////////////
	meta.feed(buff, buff_size);
	for (auto it = meta.headers.begin(); it != meta.headers.end(); ++it) {
		string_array_t strings;
		parser::extract_strings(*it, strings);
		switch ((*it)->type) {
		// ...
			case types::memory_device: {
				auto x = static_cast<mem_device *>(*it);
				if (x->total_width == 0)
					break;
				printf("Memory device %s (%s):\n", strings[x->device_locator], strings[x->bank_locator]);
				printf("  Data width:    %d bit\n", x->data_width);
				printf("  Size:          %d M\n",   x->size);
				printf("  Speed:         %d MHz\n", x->speed);
				printf("  Clock speed:   %d MHz\n", x->clock_speed);
				printf("  Manufacturer:  %s\n",     strings[x->manufacturer]);
				printf("  S/N:           %s\n",     strings[x->serial_number]);
				printf("  Tag number:    %s\n",     strings[x->assert_tag]);
				printf("  Part number:   %s\n",     strings[x->part_number]);
				printf("\n");
			} break;
		// ...
		// Handle all other struct types
		// ...
			default: printf("Unknown structure of type %d\n", (*it)->type);
		}
	}
	//////////////////
	// Free buffers //
	//////////////////
*/
