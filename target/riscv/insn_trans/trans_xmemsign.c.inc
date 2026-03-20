#define REQUIRE_XMEMSIGN(ctx) do {     \
    if (!ctx->cfg_ptr->ext_xmemsign) { \
        return false;                  \
    }                                  \
} while (0)

static bool trans_msign(DisasContext *ctx, arg_memsign *a)
{
    REQUIRE_XMEMSIGN(ctx);
    return false;
}

static bool trans_mverify(DisasContext *ctx, arg_memsign *a)
{
    REQUIRE_XMEMSIGN(ctx);
    return false;
}
