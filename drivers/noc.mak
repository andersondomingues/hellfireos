noc:
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/drivers/noc/ni_sulphane.c \
		$(SRC_DIR)/drivers/noc/noc.c \
		$(SRC_DIR)/drivers/noc/noc_rpc.c
