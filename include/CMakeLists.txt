cmake_minimum_required(VERSION 2.6)

install(
    DIRECTORY pushkin
    DESTINATION include
    FILES_MATCHING PATTERN *.hpp PATTERN *.inl
)
