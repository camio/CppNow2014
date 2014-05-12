NAME:=examples
include ../smake/app.mk

QMAKEDEFS = \
 BOOST_PATH="$(BOOST_PATH)" \
 SANI_PATH="$(SANI_PATH)" \
 SBASE_PATH="$(SBASE_PATH)"

# We depend on all of these because use of the POST_TARGETDEFS command in qmake
# requires them to be there.
$(eval $(call add-library-dependency,$(SANI_PATH),sani))
$(eval $(call add-library-dependency,$(SBASE_PATH),sbase))

$(eval $(call add-run-target,1,1) )
$(eval $(call add-run-target,2,2) )
$(eval $(call add-run-target,3,3) )
$(eval $(call add-run-target,4,4) )
$(eval $(call add-run-target,5,5) )
$(eval $(call add-run-target,6,6) )
