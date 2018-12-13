
// This file deals with the user interface of index.html.

function laTeX2html(str) {
   // replaces LaTeX commands for logical symbols by what is set in translator.logSymbols (currently <img>s of the symbols).
   str = str.replace(/\s*/g, '');
   str = str.replace(/\\forall[\{ ]?\}?/g, translator.logSymbols[tc.UNIVERSAL]);
   str = str.replace(/\\exists[\{ ]?\}?/g, translator.logSymbols[tc.EXISTENTIAL]);
   str = str.replace(/(\\neg|\\lnot)[\{ ]?\}?/g, translator.logSymbols[tc.NEGATION]);
   str = str.replace(/(\\vee|\\lor)[\{ ]?\}?/g, translator.logSymbols[tc.DISJUNCTION]);
   str = str.replace(/(\\wedge|\\land)[\{ ]?\}?/g, translator.logSymbols[tc.CONJUNCTION]);
   str  = str.replace(/(\\to|\\rightarrow)[\{ ]?\}?/g, translator.logSymbols[tc.IMPLICATION]);
   str = str.replace(/\\leftrightarrow[\{ ]?\}?/g, translator.logSymbols[tc.BIIMPLICATION]);
   str = str.replace(/([^'])(\\[^<]*)/, '$1<span class="latex">$2</span>'); // unfinished latex commands
   str = str.replace(/^(\\[^<]*)/, '<span class="latex">$1</span>'); // unfinished latex commands
   return str;
}

document.forms[0].FormulaInput.oninput = updateFormula;

function updateFormula() {
    var field = document.forms[0] && document.forms[0].FormulaInput;
   if (field) {
       var str = field.value;
       str = str.replace(/\*/g, translator.logSymbols[tc.UNIVERSAL]);
       str = str.replace(/E/g, translator.logSymbols[tc.EXISTENTIAL]);
       str = str.replace(/!/g, translator.logSymbols[tc.NEGATION]);
       str = str.replace(/v/g, translator.logSymbols[tc.DISJUNCTION]);
       str = str.replace(/&/g, translator.logSymbols[tc.CONJUNCTION]);
       str = str.replace(/=/g, translator.logSymbols[tc.BIIMPLICATION]);
       str = str.replace(/>/g, translator.logSymbols[tc.IMPLICATION]);

       var startSelection = this.selectionStart, endSelection = this.selectionEnd;
       field.value = str;
       field.setSelectionRange(startSelection, endSelection);
   }
}

document.forms[0].FormulaInput.insertAtCaret = function(str) {
   if (document.selection) {
      // Internet Explorer
      this.focus();
      sel = document.selection.createRange();
      sel.text = str;
      this.focus();
   }
   else if (this.selectionStart || this.selectionStart === 0) {
      // Firefox and Webkit
      var startPos = this.selectionStart;
      var endPos = this.selectionEnd;
      var scrollTop = this.scrollTop;
      this.value = this.value.substring(0, startPos)+str+this.value.substring(endPos,this.value.length);
      this.focus();
      this.selectionStart = startPos + str.length;
      this.selectionEnd = startPos + str.length;
      this.scrollTop = scrollTop;
   }
   else {
      this.value += str;
      this.focus();
   }
   renderFormula();
}


onload = function(e) {
   document.forms[0].onsubmit = function(e) {
      // The action begins...
      try {
         translator.init();
         var formula = translator.latex2fla(translator.html2latex(this.FormulaInput.value));
         if (!formula) {
             $("#formula-input").addClass("invalid");
             document.getElementById("rootAnchor").style.display = "none";
             document.getElementById("statusBox").style.display = "none";
             console.log(translator.error);
            // alert("Invalid formula.\n"+translator.error);
            return false;
         }
         $("#formula-input").removeClass("invalid");
         document.getElementById("examples").style.display = "none";
         document.getElementById("model").style.display = "none";
         document.getElementById("rootAnchor").style.display = "none";
         document.getElementById("statusBox").style.display = "inline";
         document.getElementById("statusHeader").innerHTML = "Proving...";
         document.getElementById("statusStop").style.display = "inline-block";
         document.getElementById("statusStop").firstChild.nodeValue = 'stop';
        //  document.getElementById("paintStop").firstChild.nodeValue = 'stop';
         // Now a free-variable tableau is created. When the proof is finished, prover.finished() is called.
         prover.start(formula.negate());
      }
      catch (e) {
         alert("Error: " + e);
      };
      return false;
   }

   prover.status = function(str) {
      // The prover dumps status messages to this function.
      document.getElementById("status").innerHTML = str;
      $("#statusCard").removeClass("red-status");
      $("#statusCard").removeClass("green-status");
   }

   prover.finished = function(treeClosed) {
      // The prover has finished.
      document.getElementById("statusHeader").innerHTML = "<span class='formula'>" + translator.fla2html(this.formula[1]) + "</span> is " + (treeClosed ? "valid." : "invalid.");
      document.getElementById("statusStop").style.display = "none";
      prover.status("");
      // Translate the free-variable tableau into a sentence tableau:
      var sentenceTree = new SenTree(this.tree);
      if (!treeClosed) {
          $("#statusCard").addClass("red-status");
          $("#statusCard").removeClass("green-status");
         // Tree is open. Display a countermodel if one is known:
         if (!this.counterModel) this.counterModel = sentenceTree.getCounterModel();
         if (this.counterModel) {
            // document.getElementById("model").style.display = "block";
            // document.getElementById("model").innerHTML = "<b>Countermodel:</b><br>" + this.counterModel;
            //return; // shouldn't display tree because if the model was found by the modelFinder, the tree is unfinished
         }
     } else {
         $("#statusCard").removeClass("red-status");
         $("#statusCard").addClass("green-status");
     }
      // Start painting the tree:
      document.getElementById("rootAnchor").style.display = "inline-block";
    //   document.getElementById("paintBar").style.display = "block";
      self.painter = new TreePainter(sentenceTree, document.getElementById("rootAnchor"));
    //   self.painter.finished = function() {
    //     //  document.getElementById("paintBar").style.display = "none";
    //   }
      self.painter.paintTree();
   }

   // event handlers for the buttons that control the proving/painting:

   document.getElementById("statusStop").onclick = function(e) {
      if (this.firstChild.nodeValue == 'stop') {
         prover.stop();
         this.firstChild.nodeValue = 'continue';
         return;
      }
      this.firstChild.nodeValue = 'stop';
      prover.nextStep();
   }

   // document.getElementById("paintStop").onclick = function(e) {
   //    if (this.firstChild.nodeValue == 'stop') {
   //       painter.stop();
   //       this.firstChild.nodeValue = 'continue';
   //       return;
   //    }
   //    this.firstChild.nodeValue = 'stop';
   //    painter.paintTree();
   // }
   //
   // document.getElementById("paintFaster").onclick = function(e) {
   //    if (this.firstChild.nodeValue == 'faster') {
   //       painter.oldInterval = painter.paintInterval;
   //       painter.paintInterval = 100;
   //       this.firstChild.nodeValue = 'slower';
   //       return;
   //    }
   //    painter.paintInterval = painter.oldInterval;
   //    this.firstChild.nodeValue = 'faster';
   // }

   // prove formula submitted via URL:
   if (location.search.match(/\?f=/)) {
      document.forms[0].FormulaInput.value = laTeX2html(unescape(location.search.substr(3)));
      document.forms[0].onsubmit();
   }
}
function changeFixedImage() {
    $($("#fixed-image > img").get(0)).attr("src", "imgs/" + Math.floor((Math.random() * 4) + 1) + ".png");//"src/" + Math.floor(Math.random() * 4) + ".png";
};
