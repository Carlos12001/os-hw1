	component cpu is
		port (
			clk_clk                                    : in  std_logic                    := 'X';             -- clk
			leds_hours_ls_external_connection_export   : out std_logic_vector(6 downto 0);                    -- export
			leds_hours_ms_external_connection_export   : out std_logic_vector(6 downto 0);                    -- export
			leds_minutes_ls_external_connection_export : out std_logic_vector(6 downto 0);                    -- export
			leds_minutes_ms_external_connection_export : out std_logic_vector(6 downto 0);                    -- export
			pio_buzzer_external_connection_export      : out std_logic;                                       -- export
			pio_key_0_external_connection_export       : in  std_logic                    := 'X';             -- export
			pio_key_1_external_connection_export       : in  std_logic                    := 'X';             -- export
			pio_switches_external_connection_export    : in  std_logic_vector(1 downto 0) := (others => 'X'); -- export
			reset_reset_n                              : in  std_logic                    := 'X'              -- reset_n
		);
	end component cpu;

	u0 : component cpu
		port map (
			clk_clk                                    => CONNECTED_TO_clk_clk,                                    --                                 clk.clk
			leds_hours_ls_external_connection_export   => CONNECTED_TO_leds_hours_ls_external_connection_export,   --   leds_hours_ls_external_connection.export
			leds_hours_ms_external_connection_export   => CONNECTED_TO_leds_hours_ms_external_connection_export,   --   leds_hours_ms_external_connection.export
			leds_minutes_ls_external_connection_export => CONNECTED_TO_leds_minutes_ls_external_connection_export, -- leds_minutes_ls_external_connection.export
			leds_minutes_ms_external_connection_export => CONNECTED_TO_leds_minutes_ms_external_connection_export, -- leds_minutes_ms_external_connection.export
			pio_buzzer_external_connection_export      => CONNECTED_TO_pio_buzzer_external_connection_export,      --      pio_buzzer_external_connection.export
			pio_key_0_external_connection_export       => CONNECTED_TO_pio_key_0_external_connection_export,       --       pio_key_0_external_connection.export
			pio_key_1_external_connection_export       => CONNECTED_TO_pio_key_1_external_connection_export,       --       pio_key_1_external_connection.export
			pio_switches_external_connection_export    => CONNECTED_TO_pio_switches_external_connection_export,    --    pio_switches_external_connection.export
			reset_reset_n                              => CONNECTED_TO_reset_reset_n                               --                               reset.reset_n
		);

