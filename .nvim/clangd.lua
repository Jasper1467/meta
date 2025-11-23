-- .nvim/clangd.lua
-- Project-specific clangd config for meta framework

local project_root = vim.fn.getcwd()

-- Automatically pick build directory in order: Debug > Release > build
local build_candidates = {"build/Debug", "build/Release", "build"}
local build_dir = nil

for _, candidate in ipairs(build_candidates) do
    local path = project_root .. "/" .. candidate
    if vim.fn.isdirectory(path) == 1 then
        build_dir = path
        break
    end
end

-- Fallback if nothing exists
if not build_dir then
    build_dir = project_root .. "/build"
end

-- Start clangd manually
vim.lsp.start_client({
    name = "clangd",
    cmd = { "clangd", "--compile-commands-dir=" .. build_dir },
    root_dir = project_root,
    capabilities = vim.lsp.protocol.make_client_capabilities(),
})

print("clangd using compile_commands.json at: " .. build_dir)

