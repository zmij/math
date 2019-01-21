cmake_minimum_required(VERSION 2.6)

install(
    DIRECTORY psst
    DESTINATION include
    FILES_MATCHING PATTERN *.hpp PATTERN *.inl
)
