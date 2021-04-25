#ifndef STATUS_HPP
#define STATUS_HPP
constexpr Status BADARGMNT{201031024436148032UL};  // bad argument
constexpr Status BADARGMNT8{201031024436148040UL};  // too many file descriptors
constexpr Status BADARGMNTb{201031024436148070UL};  // a branch name must be a short name
constexpr Status BADARGMNTE{201031024436148046UL};  // unrecognized extra arguments
constexpr Status BADARGMNTi{201031024436148077UL};  // bad URL
constexpr Status BADARGMNTL{201031024436148053UL};  // expect a Lamport UUID
constexpr Status BADARGMNTl{201031024436148080UL};  // name is too long; limit: 10 Base64 chars
constexpr Status BADARGMNTR{201031024436148059UL};  // RON variant not understood
constexpr Status BADARGMNTr{201031024436148086UL};  // wrong/unrelated replica
constexpr Status BADARGMNTs{201031024436148087UL};  // a form name must be a short name
constexpr Status BADARGMNTt{201031024436148088UL};  // the buffer is too short
constexpr Status BADARGMNTU{201031024436148062UL};  // argument not understood
constexpr Status BADARGMNTu{201031024436148089UL};  // unknown command
constexpr Status BADARGMNTV{201031024436148063UL};  // incorrect UUID version
constexpr Status BADARGMNTx{201031024436148092UL};  // unrecognized extra arguments
constexpr Status BADRONBOX{201032189560588352UL};  // 
constexpr Status BADRONBOXJ{201032189560588371UL};  // the box must be joined to the previous one
constexpr Status BADCOMAND{201031158751392576UL};  // 
constexpr Status BADFORMAT{201031364996802368UL};  // 
constexpr Status BADFORMAT1{201031364996802369UL};  // magic bytes don not match
constexpr Status BADFORMATa{201031364996802405UL};  // wrong UUID type
constexpr Status BADFORMATB{201031364996802379UL};  // branch id is up to 10 Base64 chars
constexpr Status BADFORMATb{201031364996802406UL};  // UUID expected
constexpr Status BADPAGE{201032036972494848UL};  // 
constexpr Status BADPAGE_E{201032036972643200UL};  // a varint page must not be empty
constexpr Status BADPAGE_S{201032036972644096UL};  // a varint must not span pages
constexpr Status BADPATH{201032037191385088UL};  // 
constexpr Status BADPATH_p{201032037191535872UL};  // the parent path is not in the repo
constexpr Status BADPATH_x{201032037191536384UL};  // the path is already in the repo
constexpr Status BADRECORD{201032178642039616UL};  // 
constexpr Status BADRECORD1{201032178642039617UL};  // wrong size varint
constexpr Status BADRECORDB{201032178642039627UL};  // box size is out of bounds
constexpr Status BADRECORDC{201032178642039628UL};  // bad checkout log entry
constexpr Status BADRECORDD{201032178642039629UL};  // invalid yarn declaration
constexpr Status BADRECORDd{201032178642039656UL};  // box descriptor is out of range
constexpr Status BADRECORDF{201032178642039631UL};  // incorrect filename format
constexpr Status BADRECORDl{201032178642039664UL};  // missing log data
constexpr Status BADRECORDL{201032178642039637UL};  // the limb is corrupted
constexpr Status BADRECORDO{201032178642039640UL};  // op format does not match the pattern
constexpr Status BADRECORDr{201032178642039670UL};  // a rough map value is shorter than expected
constexpr Status BADRECORDR{201032178642039643UL};  // the rough map is corrupted
constexpr Status BADRECORDS{201032178642039644UL};  // no spec in a RONv op
constexpr Status BADRECORDT{201032178642039645UL};  // the index twig seems damaged
constexpr Status BADRECORDU{201032178642039646UL};  // bad UTF-8
constexpr Status BADRECORDv{201032178642039674UL};  // mismatching varint
constexpr Status BADRECORDV{201032178642039647UL};  // the varint stream is corrupted
constexpr Status BADREPLY{201032178859384832UL};  // 
constexpr Status BADSIZE{201032252039626752UL};  // 
constexpr Status BADSTATE{201032263435345920UL};  // 
constexpr Status BADSTATE_C{201032263435348236UL};  // concurrent changes detected
constexpr Status BADSTATE_c{201032263435348263UL};  // there are uncommitted changes
constexpr Status BADSTATE_F{201032263435348239UL};  // unclosed frame
constexpr Status BADSTATE_O{201032263435348248UL};  // already open
constexpr Status BADSTATE_R{201032263435348251UL};  // failed to reach the root
constexpr Status BADSTATE_T{201032263435348253UL};  // the branch is torn (the last recorded op is not local)
constexpr Status BADSTATE_o{201032263435348275UL};  // operation is not supported
constexpr Status BADSTATE_x{201032263435348284UL};  // invlid concurrent write
constexpr Status BADSTATE_t{201032263435348280UL};  // the file is apparently touched
constexpr Status BADSTATE_w{201032263435348283UL};  // not writable
constexpr Status BADSYNTAX{201032269022144576UL};  // 
constexpr Status BADSYNTAXE{201032269022144590UL};  // can not spread non-event ops
constexpr Status BADSYNTAXF{201032269022144591UL};  // invalid float
constexpr Status BADSYNTAXI{201032269022144594UL};  // invalid integer
constexpr Status BADSYNTAXi{201032269022144621UL};  // malformed branch id
constexpr Status BADSYNTAXR{201032269022144603UL};  // invalid RONt syntax
constexpr Status BADSYNTAXS{201032269022144604UL};  // invalid RONt spread
constexpr Status BADSYNTAXU{201032269022144606UL};  // invalid UUID syntax
constexpr Status BADSYNTAXJ{201032269022144595UL};  // invalid JSON syntax
constexpr Status BADTYPE{201032337771200512UL};  // 
constexpr Status BADUIDELT{201032389109569344UL};  // 
constexpr Status BADUNICODE{201032394561651534UL};  // 
constexpr Status BADUUID{201032402078007296UL};  // 
constexpr Status ISECLIPSED{332202892660687757UL};  // eclipsed by a concurrent value
constexpr Status ENDOFRAME{258734344031003520UL};  // end of input
constexpr Status ENDOFRAME1{258734344031003521UL};  // end of a FACT frame
constexpr Status ENDOFRAME2{258734344031003522UL};  // end of a BLOB frame
constexpr Status ENDOFRAME3{258734344031003523UL};  // end of a QUERY frame
constexpr Status ENDOFRAMEE{258734344031003534UL};  // end of file or stream
constexpr Status ENDOFRAMEB{258734344031003531UL};  // end of a blob
constexpr Status ERRNO{259921757077504000UL};  // 
constexpr Status FAILDB{273111349498740736UL};  // 
constexpr Status FAILEXEC{273111350945300480UL};  // 
constexpr Status FAILEXEC_r{273111350945302838UL};  // remote execution failed
constexpr Status FAILINOUT{273111355075192640UL};  // 
constexpr Status FAILINOUTC{273111355075192652UL};  // can not close a file?!
constexpr Status FAILINOUTc{273111355075192679UL};  // can not create a file
constexpr Status FAILINOUTD{273111355075192653UL};  // getcwd fails
constexpr Status FAILINOUTl{273111355075192688UL};  // can not list a directory
constexpr Status FAILINOUTO{273111355075192664UL};  // can not open a file
constexpr Status FAILINOUTR{273111355075192667UL};  // read fails
constexpr Status FAILINOUTS{273111355075192668UL};  // can not fsync
constexpr Status FAILINOUTW{273111355075192672UL};  // write fails
constexpr Status FAILLOCK{273111358310006784UL};  // 
constexpr Status FAILMERGE{273111359219893120UL};  // 
constexpr Status FAILMMAP{273111359349690368UL};  // 
constexpr Status FAILMMAP_U{273111359349692702UL};  // can not unmap memory
constexpr Status FAILNAVBLK{273111360227554644UL};  // 
constexpr Status FAILNET{273111360294092800UL};  // 
constexpr Status FAILNOOBJ{273111360460600512UL};  // 
constexpr Status FAILNOOP{273111360460656640UL};  // 
constexpr Status FAILNOYARN{273111360463218391UL};  // no such yarn
constexpr Status FAILNOYID{273111360463250240UL};  // need a yarn id (non-zero)
constexpr Status FAILOBJGAP{273111361314947737UL};  // 
constexpr Status FAILOPEN{273111361548546048UL};  // 
constexpr Status FAILOPEN_1{273111361548548353UL};  // can not open the file
constexpr Status FAILOPEN_P{273111361548548377UL};  // fork() failed
constexpr Status FAILOPFMT{273111361548805952UL};  // 
constexpr Status FAILOPFMT1{273111361548805953UL};  // yarn object root must have RDT `yarn`
constexpr Status FAILOPFMTE{273111361548805966UL};  // event id expected (YOLO)
constexpr Status FAILOPFMTO{273111361548805976UL};  // zero yarn id is not allowed (except in @0-0-0)
constexpr Status FAILOPFMTR{273111361548805979UL};  // root ref must be an RDT (short name)
constexpr Status FAILOPFMTr{273111361548806006UL};  // yarn root is also an object root
constexpr Status FAILREF{273111364585390080UL};  // 
constexpr Status FAILREF_E{273111364585538432UL};  // home index 0 must be a root
constexpr Status FAILREF_H{273111364585538624UL};  // reverse order of home indices
constexpr Status FAILREF_R{273111364585539264UL};  // op's ref is not found
constexpr Status FAILREPET{273111364588070720UL};  // 
constexpr Status FAILREPET1{273111364588070721UL};  // the op was already seen
constexpr Status FAILREPET2{273111364588070722UL};  // the yarn was already seen
constexpr Status FAILRESIZE{273111364588873934UL};  // can not resize a file
constexpr Status FAILROOT{273111364755640320UL};  // 
constexpr Status FAILVALUES{273111368814945180UL};  // 
constexpr Status FAILVARINT{273111368816469469UL};  // 
constexpr Status FAILYRNGAP{273111372321850009UL};  // 
constexpr Status FAILZIP{273111373245054976UL};  // 
constexpr Status NEEDARG{418334292590985216UL};  // 
constexpr Status NEQFLOAT{418387218347446272UL};  // 
constexpr Status NEQFRAMLEN{418387224558130071UL};  // 
constexpr Status NEQID{418387415510548480UL};  // 
constexpr Status NEQINT{418387426734505984UL};  // 
constexpr Status NEQREF{418388035311239168UL};  // 
constexpr Status NEQSTRING{418388120342983680UL};  // 
constexpr Status NEQUUID{418388258703278080UL};  // 
constexpr Status NEQVALCNT{418388305998083904UL};  // 
constexpr Status NEQVALTYPE{418388306002585166UL};  // 
constexpr Status NOACCESS{421131383385210880UL};  // 
constexpr Status NOACCESS_r{421131383385213238UL};  // the path is outside the repo
constexpr Status NOCOMMIT{421141014980972544UL};  // 
constexpr Status NODATA{436050722992160768UL};  // no more data available
constexpr Status BADSEAL{201032247327064064UL};  // the sealing signature does not match the data
constexpr Status OLDSEAL{438315652694147072UL};  // the old sealing signature matches
constexpr Status NODIR{421145005705396224UL};  // 
constexpr Status NOSPACE{421211439390785536UL};  // 
constexpr Status NOTAVAILB{421214829161304768UL};  // 
constexpr Status NOTFOUND{421215165579317248UL};  // 
constexpr Status NOTFOUND0a{421215165579317285UL};  // no repo found
constexpr Status NOTFOUND0b{421215165579317286UL};  // missing argument
constexpr Status NOTFOUND0C{421215165579317260UL};  // no such checkout
constexpr Status NOTFOUND0d{421215165579317288UL};  // not a directory
constexpr Status NOTFOUND0F{421215165579317263UL};  // expected a FLOAT
constexpr Status NOTFOUND0f{421215165579317290UL};  // file not found
constexpr Status NOTFOUND0I{421215165579317266UL};  // expected an INT
constexpr Status NOTFOUND0L{421215165579317269UL};  // limb not found
constexpr Status NOTFOUND0P{421215165579317273UL};  // new path, no file created yet
constexpr Status NOTFOUND0p{421215165579317300UL};  // path not found
constexpr Status NOTFOUND0x{421215165579317308UL};  // peer not found
constexpr Status NOTFOUND0R{421215165579317275UL};  // no such record
constexpr Status NOTFOUND0S{421215165579317276UL};  // expected a STRING
constexpr Status NOTFOUND0T{421215165579317277UL};  // not found in a trie
constexpr Status NOTFOUND0U{421215165579317278UL};  // expected an UUID
constexpr Status NOTIMPLTD{421215369505919808UL};  // 
constexpr Status NOTIMPLTDf{421215369505919850UL};  // form unknown
constexpr Status NOTIMPLTDp{421215369505919860UL};  // projection unknown
constexpr Status NOTIMPLTDP{421215369505919833UL};  // protocol not implemented
constexpr Status NOTOK_C{421215779857367040UL};  // the working copy has changes; stash or drop
constexpr Status NOTOPEN{421215784859860992UL};  // 
constexpr Status NOTOPEN_b{421215784860010880UL};  // no open branch
constexpr Status NOTOPEN_R{421215784860010176UL};  // file not open for reading
constexpr Status NOTOPEN_W{421215784860010496UL};  // file not open for writing
constexpr Status NOTREADY{421215979137540096UL};  // 
constexpr Status NOTSAVED{421216043914547200UL};  // 
constexpr Status OUTOMEMRY{440919030008232064UL};  // 
constexpr Status OUTORANGE{440919035310048128UL};  // out of range
constexpr Status OUTORANGEB{440919035310048139UL};  // the position is greater than the branch size
constexpr Status OUTORANGEL{440919035310048149UL};  // no such line
constexpr Status OUTORANGEl{440919035310048176UL};  // the position is not in the log
constexpr Status OUTORANGEO{440919035310048152UL};  // home index is not valid
constexpr Status OUTORANGER{440919035310048155UL};  // root is reached, can not go further
constexpr Status SQLITE{511815134960680960UL};  // 
constexpr Status WARNSHORT{579395860230682432UL};  // 
using ErrorList = std::vector<std::pair<Status, const Codepoint*>>;
extern ErrorList ERRORS;
#endif
