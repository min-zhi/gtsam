function varargout = serialize(varargin)
      if length(varargin) == 0
        varargout{1} = geometry_wrapper(95, varargin{:});
      else
        error('Arguments do not match any overload of function serialize');
      end