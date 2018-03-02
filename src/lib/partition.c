/* =============================================================================
 * Pure64 -- a 64-bit OS/software loader written in Assembly for x86-64 systems
 * Copyright (C) 2008-2017 Return Infinity -- see LICENSE.TXT
 * =============================================================================
 */

#include <pure64/partition.h>

#include <pure64/error.h>

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

static int partition_read(void *partition_ptr, void *buf, uint64_t buf_size) {

	struct pure64_partition *partition = (struct pure64_partition *) partition_ptr;
	if ((partition == NULL) || (partition->disk == NULL))
		return PURE64_EFAULT;

	return pure64_stream_read(partition->disk, buf, buf_size);
}

static int partition_write(void *partition_ptr, const void *buf, uint64_t buf_size) {

	struct pure64_partition *partition = (struct pure64_partition *) partition_ptr;
	if ((partition == NULL)|| (partition->disk == NULL))
		return PURE64_EFAULT;

	return pure64_stream_write(partition->disk, buf, buf_size);
}

static int partition_set_pos(void *partition_ptr, uint64_t pos) {

	struct pure64_partition *partition = (struct pure64_partition *) partition_ptr;
	if ((partition == NULL)|| (partition->disk == NULL))
		return PURE64_EFAULT;

	return pure64_stream_set_pos(partition->disk, partition->offset + pos);
}

static int partition_get_pos(void *partition_ptr, uint64_t *pos) {

	struct pure64_partition *partition = (struct pure64_partition *) partition_ptr;
	if ((partition == NULL)|| (partition->disk == NULL))
		return PURE64_EFAULT;

	int err = pure64_stream_get_pos(partition->disk, pos);
	if (err != 0)
		return err;

	if (pos != NULL)
		*pos -= partition->offset;

	return 0;
}

static int partition_get_size(void *partition_ptr, uint64_t *size) {

	struct pure64_partition *partition = (struct pure64_partition *) partition_ptr;
	if (partition == NULL)
		return PURE64_EFAULT;

	if (size != NULL)
		*size = partition->size;

	return 0;
}

void pure64_partition_init(struct pure64_partition *partition) {
	pure64_stream_init(&partition->stream);
	partition->stream.data = partition;
	partition->stream.read = partition_read;
	partition->stream.write = partition_write;
	partition->stream.set_pos = partition_set_pos;
	partition->stream.get_pos = partition_get_pos;
	partition->stream.get_size = partition_get_size;
	partition->disk = NULL;
	partition->offset = 0;
	partition->size = 0;
}

void pure64_partition_done(struct pure64_partition *partition) {
	/* Currently nothing to do here. */
	(void) partition;
}

void pure64_partition_set_stream(struct pure64_partition *partition,
                                 struct pure64_stream *disk) {

	if (disk != NULL)
		partition->disk = disk;
}

void pure64_partition_set_size(struct pure64_partition *partition,
                               uint64_t size) {

	partition->size = size;
}

void pure64_partition_set_offset(struct pure64_partition *partition,
                                 uint64_t offset) {

	partition->offset = offset;
}
