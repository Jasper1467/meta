-- .nvim/clangd.lua
-- Project-specific clangd config for MetaProject framework

local vim = vim
local fn = vim.fn
local fs = vim.fs

-- Prefer .git or CMakeLists.txt as root marker
local root_dir = fs.find({"CMakeLists.txt", ".git"}, { upward = true })[1] or fn.getcwd()

-- Automatically pick build directory in order: Debug > Release > build
local build_candidates = { "build/Debug", "build/Release", "build" }
local build_dir = nil
for _, candidate in ipairs(build_candidates) do
    local path = root_dir .. "/" .. candidate
    if fn.isdirectory(path) == 1 then
        build_dir = path
        break
    end
end
-- fallback
if not build_dir then
    build_dir = root_dir .. "/build"
end

-- clangd flags tailored for MetaProject
local clangd_cmd = {
    "clangd",
    "--compile-commands-dir=" .. build_dir,
    "--clang-tidy",               -- enable clang-tidy
    "--header-insertion=never",   -- avoid auto-inserting headers
    "--cross-file-rename",        -- better rename support
    "--completion-style=detailed"
}

-- Apply clang-format automatically for C/C++ files
vim.api.nvim_create_autocmd("BufWritePre", {
    pattern = { "*.cpp", "*.hpp", "*.cxx", "*.h" },
    callback = function()
        vim.cmd("silent! !clang-format -i %")
    end
})

-- ==============================
-- Start clangd Client
-- ==============================
vim.lsp.start_client({
    name = "clangd",
    cmd = clangd_cmd,
    root_dir = root_dir,
    capabilities = vim.lsp.protocol.make_client_capabilities(),
})
