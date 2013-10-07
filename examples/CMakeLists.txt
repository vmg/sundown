cmake_minimum_required(VERSION 2.8)

set(EXAMPLES
	sundown
	smartypants
)

# Create static-linked executables
foreach(example ${EXAMPLES})
	add_executable(${example} ${example}.c)
	target_link_libraries(${example} sundown-static)
endforeach(example)
