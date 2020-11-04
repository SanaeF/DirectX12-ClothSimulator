#include "DxHelper.h"

size_t DxHelper::AlignmentedSize(size_t size, size_t alignment) {
	return size + alignment - size % alignment;
}