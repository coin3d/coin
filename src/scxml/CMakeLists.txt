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
