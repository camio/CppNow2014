NAME:=testdriver
include ../smake/app.mk

QMAKEDEFS = \
 BOOST_PATH="$(BOOST_PATH)" \
 SBASE_PATH="$(SBASE_PATH)"

# We depend on all of these because use of the POST_TARGETDEFS command in qmake
# requires them to be there.
$(eval $(call add-library-dependency,$(SBASE_PATH),sbase))
$(eval $(call add-boost-dependency,unit_test_framework))
$(eval $(call add-boost-dependency,serialization))
