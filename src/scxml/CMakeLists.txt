if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
        scxml/SbStringConvert.cpp
        scxml/ScXML.cpp
        scxml/ScXMLObject.cpp
        scxml/ScXMLEvent.cpp
        scxml/ScXMLEventTarget.cpp
        scxml/ScXMLStateMachine.cpp
        scxml/ScXMLDocument.cpp
        scxml/ScXMLElt.cpp
        scxml/ScXMLScxmlElt.cpp
        scxml/ScXMLInitialElt.cpp
        scxml/ScXMLAbstractStateElt.cpp
        scxml/ScXMLStateElt.cpp
        scxml/ScXMLParallelElt.cpp
        scxml/ScXMLFinalElt.cpp
        scxml/ScXMLOnEntryElt.cpp
        scxml/ScXMLOnExitElt.cpp
        scxml/ScXMLTransitionElt.cpp
        scxml/ScXMLHistoryElt.cpp
        scxml/ScXMLExecutableElt.cpp
        scxml/ScXMLEventElt.cpp
        scxml/ScXMLIfElt.cpp
        scxml/ScXMLElseIfElt.cpp
        scxml/ScXMLElseElt.cpp
        scxml/ScXMLLogElt.cpp
        scxml/ScXMLDataModelElt.cpp
        scxml/ScXMLDataElt.cpp
        scxml/ScXMLAssignElt.cpp
        scxml/ScXMLValidateElt.cpp
        scxml/ScXMLSendElt.cpp
        scxml/ScXMLInvokeElt.cpp
        scxml/ScXMLParamElt.cpp
        scxml/ScXMLFinalizeElt.cpp
        scxml/ScXMLContentElt.cpp
        scxml/ScXMLAnchorElt.cpp
        scxml/ScXMLScriptElt.cpp
        scxml/ScXMLEvaluator.cpp
        scxml/ScXMLMinimumEvaluator.cpp
        scxml/eval-minimum.cpp
        scxml/eval-minimum-tab.cpp
        scxml/ScXMLXPathEvaluator.cpp
        scxml/ScXMLECMAScriptEvaluator.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(scxml OBJECT
        SbStringConvert.cpp
        
        ScXML.cpp
        ScXMLObject.cpp
        ScXMLEvent.cpp
        ScXMLEventTarget.cpp
        ScXMLStateMachine.cpp
        ScXMLDocument.cpp
        
        ScXMLElt.cpp
        ScXMLScxmlElt.cpp
        ScXMLInitialElt.cpp
        ScXMLAbstractStateElt.cpp
        ScXMLStateElt.cpp
        ScXMLParallelElt.cpp
        ScXMLFinalElt.cpp
        ScXMLOnEntryElt.cpp
        ScXMLOnExitElt.cpp
        ScXMLTransitionElt.cpp
        ScXMLHistoryElt.cpp
        
        ScXMLExecutableElt.cpp
        ScXMLEventElt.cpp
        ScXMLIfElt.cpp
        ScXMLElseIfElt.cpp
        ScXMLElseElt.cpp
        ScXMLLogElt.cpp
        
        ScXMLDataModelElt.cpp
        ScXMLDataElt.cpp
        ScXMLAssignElt.cpp
        ScXMLValidateElt.cpp
        
        ScXMLSendElt.cpp
        ScXMLInvokeElt.cpp
        ScXMLParamElt.cpp
        ScXMLFinalizeElt.cpp
        ScXMLContentElt.cpp
        
        ScXMLAnchorElt.cpp
        
        ScXMLScriptElt.cpp
        
        ScXMLEvaluator.cpp
        
        ScXMLMinimumEvaluator.cpp
        eval-minimum.cpp
        eval-minimum-tab.cpp
        
        ScXMLXPathEvaluator.cpp
        ScXMLECMAScriptEvaluator.cpp
	)
endif()

file(GLOB_RECURSE SCXMLCPP ScXML*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SCXMLCPP}
	${CMAKE_SOURCE_DIR}/src/scxml/ScXMLP.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)
