#define REQUIRE_XMEMSIGN(ctx) do {     \
    if (!ctx->cfg_ptr->ext_xmemsign) { \
        return false;                  \
    }                                  \
} while (0)

static bool trans_msign(DisasContext *ctx, arg_memsign *a)
{
    REQUIRE_XMEMSIGN(ctx);

    /* Get the pointer and length of the target memory */
    TCGv src_mem = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv src_len = tcg_constant_tl(a->imm);

    /* Calculate the signature and put it in rd */
    TCGv dst_sign = dest_gpr(ctx, a->rd);
    gen_helper_msign(dst_sign, tcg_env, src_mem, src_len);
    gen_set_gpr(ctx, a->rd, dst_sign);

    return true;
}

static bool trans_mverify(DisasContext *ctx, arg_memsign *a)
{
    REQUIRE_XMEMSIGN(ctx);

    /* Get the pointer and length of the target memory */
    TCGv src_mem = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv src_len = tcg_constant_tl(a->imm);

    /* Get the expected signature from rd */
    TCGv old_sign = get_gpr(ctx, a->rd, EXT_NONE);

    /* Put the verification result in rd */
    TCGv dst_val = dest_gpr(ctx, a->rd);
    gen_helper_mverify(dst_val, tcg_env, src_mem, src_len, old_sign);
    gen_set_gpr(ctx, a->rd, dst_val); // 0/1

    return true;
}
