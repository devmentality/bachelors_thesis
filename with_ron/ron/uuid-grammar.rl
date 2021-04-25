%%{

    machine RONID;
    alphtype unsigned char;

    action begin_ron64 {
        pd = p;
    }
    action hi {
        CALL(Uuid::ParseIndex( id.UpperIndex(), span8c{pd,p} ));
    }
    action yi {
        CALL(Uuid::ParseIndex( id.LowerIndex(), span8c{pd,p} ));
    }
    action fw {
        CALL(Uuid::ParseWord( id.first, span8c{pd,p} ));
    }
    action YOLO {
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= YOLO_FLAGS;
    }
    action LAMPORT {
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= LAMPORT_FLAGS;
    }
    action TAG {
        CALL(Uuid::ParseWord( id.second, span8c{pd,p} ));
        id.second |= TAG_FLAGS;
    }
    action NAME {
        CALL(Uuid::ParseDouble( id, span8c{pd,p} ));
        id.second |= NAME_FLAGS;
    }

    RON64 = [0-9a-zA-Z~_]+ >begin_ron64;

    # using the lengths in the grammar would double the
    # parser size due to combinatorial state explosion
    HALF = RON64;   # 1..5
    WORD = RON64;   # 1..10
    DOUBLE = RON64; # 1..20

    RONID = (
          (HALF '-' @hi HALF '-' @yi WORD )  %YOLO
        | (WORD '+' @fw WORD              )  %LAMPORT
        | (WORD '^' @fw WORD              )  %TAG
        | (DOUBLE                         )  %NAME
    ) ;

}%%
