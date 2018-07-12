include(GraphViz)

#graphviz_generate_png(PNG_FILES
#  )

add_custom_target(wip_comms_graphviz ALL
  DEPENDS ${PNG_FILES}
  VERBATIM )
