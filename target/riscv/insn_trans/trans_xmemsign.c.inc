#define REQUIRE_XMEMSIGN(ctx) do {     \
    if (!ctx->cfg_ptr->ext_xmemsign) { \
        return false;                  \
    }                                  \
} while (0)

static bool trans_msign(DisasContext *ctx, arg_memsign *a)
{
    REQUIRE_XMEMSIGN(ctx);
    qemu_log("DEBUG: Decoding MSIGN\n");
    return true;
}

static bool trans_mverify(DisasContext *ctx, arg_memsign *a)
{
    REQUIRE_XMEMSIGN(ctx);
    qemu_log("DEBUG: Decoding MVERIFY\n");
    return true;
}
